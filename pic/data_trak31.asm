;
;		DataTrak Serial TX for 16F628
;		Drop in replacement for TinyTrak
;		1200 baud AX.25 transmit
;		9k6 int async serial Rx/Tx
;		User programmable calls/path/txd
;		User friendly setup menu with text echo toggle
;		CD indication while listening fixed 
;		10 MHz OSC
;		11/20/04	    V3.1	
;					
;------------------------------------------------------------
        LIST    P=16F628
	ERRORLEVEL	-302
        include <p16f628.inc>
        __CONFIG        h'3f22'
;-------------------------------------------------------------
;               Port Set-up
;           0   1   2   3   4   5   6   7       bit
; port  A   Out Out Out Out In  X   X   X	
;       B   Out Out Out Out In  In  In  In	
;------------------------------------------------------------
	ORG	0			; start code
	goto	main
	ORG	4
	goto	int_service		;interrupt vector
;------------------------------------------------------------
        cblock  0x20
                temp_S                  ; 20
                temp_W                  ; int regs
;
		txd_count		; txd buffer
                temp                    ; buffer
                count                   ; byte counter
		offset			; lookup counter	
                flag_count              ; flag counter
                send_reg                ; data byte
                bit_count               ; data bit cntr
                bit_stuff_count         ; bit stuffing
                AX25_crc_hi             ; for CRC
                AX25_crc_lo             ; for CRC
                next_freq               ; 0/1 tone / cfg routine 
                loop_count              ; loop down counter
                pir                     ; phase inc reg
                FLAGS                   ; various flag bits
		buffer_1		; (30)buffer
		last_look		; compare reg
		ee_addr			; 
		ee_data			; 
		rx_byte			; serial regs
		rx_buffer		; 
		rx_count		; 
		tx_byte			; 
		tx_buffer		; 
		tx_count		; (39)
        endc
;------------------------------------------------------------
#define TONE            FLAGS,0         ;tone freq hi/lo
#define	ERR_FLAG	FLAGS,1		;error flag
#define	AH_FLAG		FLAGS,2		;ascii hex error
#define	BT_FLAG		FLAGS,3		;done entering calls
;
#define	CD		PORTA,0		;CD LED
#define	GPS		PORTA,1		;GPS LED
#define PTT_OUT         PORTA,2         ;PTT when set
#define	SERIAL_OUT	PORTA,3		;serial data out 
#define SERIAL_IN       PORTA,4         ;serial data in
#define PTT_IN          PORTB,5         ;PTT sense (unused)
#define SW_1            PORTB,6         ;SW1 in
;------------------------------------------------------------
ONE_BIT         equ     d'129'          ;104 uS (tweak)
;
LO_TONE	 	equ	d'130'		;1200 Hz(833uS)
;
HI_TONE	 	equ	d'71'		;2200 Hz(454uS)
;
BIT_T		equ	d'63454'	;832.8 uS to overflow
;
RAML_START	equ	0x40		;low RAM start address
RAML_END	equ	0x7f		;low RAM end 
RAMM_START	equ	0xa0		;mid RAM start address
RAMM_END	equ	0xef		;mid RAM end
BANK2_START	equ	0x20		;callsign loc in bank 2
BANK2_END	equ	0x4f		;last byte bank 2
SCRATCH_PAD	equ	0x70		;temp data loc
TXD_EE		equ	0x40		;txd loc in eemem
ECHO_EE		equ	0x41		;echo setting
BACKSPACE	equ	0x08		;backspace
LF		equ	0x0a		;linefeed
CR		equ	0x0d		;carriage return
CD_MASK		equ	b'00010000'	;B:4
PACKET_ON	equ	b'11000000'	;PIE enable
ASYNC_ON	equ	b'10100000'	;T0IE enable
;------------------------------------------------------------
;
sine_table
        addwf   PCL,F                   ;add offset to pc
	dt	0x00,0x01,0x02,0x05,0x08,0x0a,0x0d,0x0e
	dt	0x0f,0x0e,0x0d,0x0a,0x08,0x05,0x02,0x01
;------------------------------------------------------------
pwrup_msg
        addwf   PCL,F
        dt      "Data Trak 2004(c)",CR,LF," Enter TOCALL",CR,LF,">",0
;------------------------------------------------------------
mycall_table
        addwf   PCL,F
        dt      CR,LF,"Enter MYCALL",CR,LF,">",0
;------------------------------------------------------------
via_1_table
	addwf	PCL,F
	dt	CR,LF,"Enter 1st VIA",CR,LF,">",0
;------------------------------------------------------------
via_2_table
        addwf   PCL,F
        dt      CR,LF,"Enter 2nd VIA",CR,LF,">",0
;------------------------------------------------------------
via_3_table
        addwf   PCL,F
        dt      CR,LF,"Enter 3rd VIA",CR,LF,">",0
;------------------------------------------------------------
txd_table
        addwf   PCL,F
        dt      CR,LF,"Enter TX Delay",CR,LF,">",0
;------------------------------------------------------------
echo_table
        addwf   PCL,F
        dt      CR,LF,"Echo text? Y/N",CR,LF,">",0
;------------------------------------------------------------
entry_done_table
        addwf   PCL,F
        dt      CR,LF,"Successful Configuration",0
;
;------------------------------------------------------------
main
        movlw   0x07
        movwf   CMCON
        bsf     STATUS,RP0              ;switch to bank 1
        movlw   b'00010000'             ;port A in/outputs
        movwf   TRISA
        movlw   b'11110000'             ;port B in/outputs
        movwf   TRISB
;
        movlw   b'00000000'             ;weak pullups on, TMR0/2
        movwf   OPTION_REG
        movlw   b'00000010'             ;TMR2IE on
        movwf   PIE1                    ;perif int setup
	movlw	LO_TONE
	movwf	PR2			;TMR2 period to 1200 Hz
        bcf     STATUS,RP0              ;back to page 0
;
        clrf    PIR1                    ;reset perif int
        movlw   b'00000000'             ;
	movwf	INTCON
        movlw   b'00000001'
        movwf   T1CON                   ;TMR1 /1, on
	movlw	b'00000000'
	movwf	T2CON			;TMR2 /1, off
        bcf     PTT_OUT			;PTT off
        bcf     GPS                     ;GPS LED on
	bcf	CD			;CD LED off
	bcf	SERIAL_OUT		;stop bit set
;
	movlw	ASYNC_ON
	movwf	INTCON			;serial ints on
	btfss	SW_1
	goto	tocall_area		;config mode
	call	load_stored_path	;ee_mem to bank 2
;------------------------------------------------------------
;	The main program begins here		
;------------------------------------------------------------
;	Parse for serial data, store in RAM and look for
;	enter or overflow to begin transmit routine.
;
start_here
	call	clean_house		;clear regs
	movlw	RAML_START
	movwf	FSR			;start storing data at RAML
;
look_for_data_loop
	btfsc	txd_count,7
	goto	echoed_text
noechoed_text
	call	serial_rx
	movfw	rx_byte		;
	movwf	INDF			;store rx'd byte
	movlw	CR			;<CR>?
	xorwf	rx_byte,W		;is it the send byte?
	bz	look_busy		;yes
	goto	storage_calculator
;
echoed_text
	call	serial_rx
	movfw	rx_byte
	movwf	INDF
	movlw	CR
	xorwf	rx_byte,W		;is it a CR? 
	bz	echo_crlf		;yes, next step
	movlw	BACKSPACE		;no, reload to W
	xorwf	rx_byte,W		;backspace?
	bnz	echo_entry		;no, do store routine
;
	movlw	RAML_START		;yes, check if at start
	xorwf	FSR,W
	bz	look_for_data_loop	;at start, do nothing
	decf	FSR,F			;prep to overwrite
	movfw	rx_byte
	movwf	tx_buffer		;send backspace 
	call	serial_tx
	movlw	" "			;overwrite blank
	movwf	tx_buffer		;
	call	serial_tx
	movfw	rx_byte
	movwf	tx_buffer		;send backspace again
	call	serial_tx
	goto	look_for_data_loop
;
echo_entry
	movfw	rx_byte			;echo rx'd byte
        movwf   tx_buffer               ;
	call	serial_tx
	goto	storage_calculator	;update FSR
echo_crlf
	movlw	LF
        movwf   tx_buffer               ;load LF to send
	call	serial_tx
	movlw	CR
        movwf   tx_buffer               ;load CR to send
	call	serial_tx
	goto	look_busy
;
storage_calculator
	movlw	RAML_END		;no
	xorwf	FSR,W			;at the low RAM end?
	bz	store_high		;yes
	incf	FSR,W			;no
	xorlw	RAMM_END		;at the hi RAM end?
	bz	look_busy		;yes, send now
	incf	FSR,F			;no, inc store offset
	goto	look_for_data_loop	;look for next
;
store_high
	movlw	RAMM_START
	movwf	FSR			;load with mid RAM offset
	goto	look_for_data_loop	;look for next
;------------------------------------------------------------
;	Looks for activity on B:4
;	Holds off TX til clear	
;
look_busy
	movlw	d'20'
	movwf	temp
	movlw	d'255'			;(1)
	movwf	buffer_1		;(1)
;
look_busy_loop
	movfw	PORTB			;(1)get PORTB
	andlw	CD_MASK			;(1)check CD
	xorwf	last_look,W		;(1)change?
	bz	busy_countdown		;(3)no
	bsf	CD			;(1)yes, LED on
	xorwf	last_look,F		;(1)save copy
	goto	look_busy		;(2)reset countdown
busy_countdown
	decfsz	buffer_1,F		;(1,2)done?
	goto	look_busy_loop		;(2)no, check CD again
	decfsz	temp,F
	goto	look_busy_loop
	bcf	CD			;(1)LED off
;------------------------------------------------------------
transmit_set_up
        clrf    TMR2                    ;
        bcf     PIR1,TMR2IF             ;clear flag
;
	movlw	b'00000100'
	movwf	T2CON			;TMR2 on
	movlw	PACKET_ON
	movwf	INTCON
;
        movlw   0xff
        movwf   AX25_crc_hi             ;reset CRC calc
        movwf   AX25_crc_lo
        bsf     PTT_OUT                 ;key radio
        movfw   txd_count		;txdelay (not cleared)
	andlw	b'01111111'		;mask bit 7
        movwf   flag_count              ;txdelay #flags
;
        movlw   high BIT_T
	movwf	TMR1H
	movlw	low BIT_T
	movwf	TMR1L
        bcf     PIR1,TMR1IF             ;clear flag
        call    tx_flags                ;send flags for tx
;
;
;------------------------------------------------------------
send_call_path
;	Path data is preconfigured and loaded from hi memory
;	Looks for (00) eof flag to end routine
;
send_address_data
	movlw	BANK2_START
	movwf	FSR			;data address in hi memory
to_list_loop
	bsf	STATUS,IRP		;bank 2,3
	movfw	INDF			;read INDF at indirect offset
	bcf	STATUS,IRP		;bank 0,1
        incf    FSR,F                   ;inc FSR
	movwf	temp
	movfw	temp			;test for zero
	bz	text_send		;saw eof flag (00f)
        call    reg_packet_out          ;send byte
	goto	to_list_loop		
;------------------------------------------------------------
text_send
        movlw   RAML_START              ;reset data pointer
        movwf   FSR
;
text_send_loop
	movlw	CR
	xorwf	INDF,W
	bz      finish_up	        ;saw <CR> EOF
        movfw   INDF
        call    reg_packet_out          ;send byte
	movlw	RAML_END
	xorwf	FSR,W
	bz	set_address_to_mid
	incf	FSR,W
	xorlw	RAMM_END
	bz	finish_up		;overrun, finish up
        incf    FSR,F                   ;inc FSR
        goto    text_send_loop          ;get next
set_address_to_mid
	movlw	RAMM_START		;set FSR to mid RAM
	movwf	FSR
	goto	text_send_loop
;
;------------------------------------------------------------
finish_up
        movlw   0xff
        xorwf   AX25_crc_hi,W
        movwf   temp                    ;inv, store hi byte
        movlw   0xff
        xorwf   AX25_crc_lo,W
        call    reg_packet_out
        movfw   temp
        call    reg_packet_out
        movlw   0x02
        movwf   flag_count
        call    tx_flags                ;send end flags
        btfss   PIR1,TMR1IF		;bit done?
        goto    $-1
;
	movlw	b'00000000'
	movwf	T2CON			;TMR2 off
        bcf     PTT_OUT                 ;un-key
	clrf	PORTB			;DAC at 0 volts
	movlw	ASYNC_ON
	movwf	INTCON
;
        goto    start_here              ;start over
;
;------------------------------------------------------------
;	Regs used	temp_W, temp_S, FLAGS, pir
;
int_service
        movwf   temp_W			;1
        swapf   STATUS,W		;1
	bcf	STATUS,RP0		;1 insure bank 0
        movwf   temp_S			;1
;
	btfsc	PIR1,TMR2IF
	goto	tone_job		;skip rest if TMR2 running
	btfsc	INTCON,T0IF
	call	bb_serial
	goto	outta_here
;
tone_job
        bcf     PIR1,TMR2IF             ;1 clear flag
        btfsc   TONE			;1,2
        goto    tone_1			;2
tone_0					;(18)
	incf	pir,F			;(1)next step
	movlw	b'00001111'		;(1)
	andwf	pir,W			;(1)
        call    sine_table		;(5)
        movwf   PORTB                   ;(1)set bits B:0-B:3
	movlw	LO_TONE			;(1)
	bsf	STATUS,RP0
	movwf	PR2			;(1)timer 2 1200 Hz
        bcf     STATUS,RP0              ;(1)bank 0
        goto    outta_here		;(2)
tone_1
	incf	pir,F			;(1)next step
	movlw	b'00001111'		;(1)
	andwf	pir,W			;(1)
        call    sine_table		;(5)
        movwf   PORTB                   ;(1)set bits B:0-B:3
	movlw	HI_TONE
	bsf	STATUS,RP0
	movwf	PR2			;timer 2 2200 Hz
        bcf     STATUS,RP0              ;bank 0
        goto    outta_here		;2
;
outta_here				;(6)
        swapf   temp_S,W		;1
        movwf   STATUS			;1
        swapf   temp_W,F		;1
        swapf   temp_W,W		;1
        retfie                          ;2 return to program
;------------------------------------------------------------
;       Subroutines
;------------------------------------------------------------
;
;               Clear registers
clean_house
        movlw   0x23
        movwf   FSR
clean_loop
        clrf    INDF
        incf    FSR,W
        xorlw   0x39
        bz     $+3
        incf    FSR,F
        goto    clean_loop
	return
;------------------------------------------------------------
;
serial_tx                               
  	movf  	tx_count,F              ;  Wait for Previous Data Sent
   	skpz
   	goto  	serial_tx               ;  Counter = 0 when Sent
	movfw	tx_buffer
	movwf	tx_byte
   	bsf    	tx_count,0              ;  Indicate there is Data to Send
  	return
;
serial_rx 
	call	check_for_CD		;CD indication 
  	btfss  	rx_count,1		;
   	goto  	serial_rx
	btfsc	rx_count,0
	goto	bad_rx			;rx_count=3, bad data
	movfw	rx_buffer
	movwf	rx_byte			;byte received
	clrf	rx_count
  	return
bad_rx
	clrf	rx_buffer
	clrf	rx_byte
	clrf	rx_count
	goto	serial_rx
;
;------------------------------------------------------------
;	Regs used: 	rx_count, rx_buffer, rx_byte
;			tx_count, tx_byte
;
bb_serial
	movlw	ONE_BIT /3
	subwf	TMR0,F			;reload 1/3 bit timeout	
	bcf	INTCON,T0IF		;clear flag                        
	movlw	0x04			; 1
	addwf	rx_count,F		; 1
	skpdc				; 1(2)
	goto	rx_no		 	; 2	
	movfw	rx_count		; 1
	xorlw	0x91			; 1
	skpnz				; 1(2)
	goto	rx_at_end		; 2
	clrc				; 1
	btfss	SERIAL_IN			; 1(2)
	setc				; 1
	rrf	rx_buffer,F		; 1
	bsf	rx_count,2		; 1
	goto	rx_done			; 2
rx_at_end
	btfsc	SERIAL_IN			; 1(2)
	goto	rx_overrun		; 2
	movlw	2			; 1
	movwf	rx_count		; 1
	goto	rx_done			; 2
;
rx_no
	btfsc	rx_count,0 		; 1(2)
	goto	rx_done			; 2
	btfss	rx_count,3		; 1(2)
	goto	rx_start_check		; 2
	btfss	SERIAL_IN			; 1(2)
	bcf	rx_count,3		; 1
	bsf	rx_count,0		; 1(2)
	btfss	rx_count,1		; 1(2)
	goto	rx_done			; 2
rx_overrun
	movlw	3			; 1
	movwf	rx_count		; 1
	goto	rx_done			; 2
rx_start_check
	btfss	SERIAL_IN		 	; 1(2)
	bcf	rx_count,2		; 1
;
rx_done
;
  	movlw  	0x04                   	; 1
  	addwf  	tx_count,F		; 1
  	skpdc                     	; 1(2)
   	goto  	tx_delaychk		; 2
  	bsf    	tx_count,2              ; 1
  	setc				; 1
  	rrf    	tx_byte,F		; 1
	bc	$+3			; 2(3)
	bsf	SERIAL_OUT			; 1
	goto	$+2 			; 2
	bcf	SERIAL_OUT			; 1
  	goto   	tx_done        		; 2
;
tx_delaychk                        	;
  	btfss  	tx_count,0              ; 1(2)
   	goto  	tx_nochk		; 2
  	movlw  	0x04                    ; 1
  	movwf  	tx_count		; 1
	bsf	SERIAL_OUT                	; 1
  	goto   	tx_done		       	; 2
;
tx_nochk                       		
  	movf   	tx_count,W		; 1
  	xorlw  	0x04                    ; 1
  	skpz				; 1(2)
   	xorlw 	0x04 ^ 0x9C   		; 1
   	skpnz				; 1(2)
    	clrf 	tx_count 		; 1
;	
tx_done
	return				; 2
;
;------------------------------------------------------------
; Regs used: 	send_reg,  bit_count, bit_stuff_count, next_freq,
;		FLAGS, AX25_crc_lo, AX25_crc_hi.			
;               Assumes byte is in W
reg_packet_out
        movwf   send_reg                ;W to send_reg
        movlw   0x08
        movwf   bit_count               ;8 bits
rotor_out
        rrf     send_reg,W              ;calc CRC
        call    crc_core
        rrf     send_reg,F              ;get bit to send
        skpc
        goto    zero_out
        incf    bit_stuff_count,F       ; b_s_c +1
        call    tx_one_bit              ;send a 1
        movfw   bit_stuff_count         ;check b_s_c
        xorlw   0x05
        skpnz                           ;b_s_f = 5 ?
        goto    zero_out                ;yes
count_the_bits
        decfsz  bit_count,F
        goto    rotor_out
        return
zero_out
        clrf    bit_stuff_count         ;reset b_s_f
        incf    next_freq,F
        call    tx_one_bit              ;send a 0
        goto    count_the_bits          ;
;
;------------------------------------------------------------
;	Regs used: 	bit_count, next_freq, flag_count.
;               Sends (7e) b'01111110' flag_count times
tx_flags
        movlw   0x06
        movwf   bit_count               ;8 bits
        incf    next_freq,F
        call    tx_one_bit              ;send 0
flag_twist
        call    tx_one_bit              ;send 6 1's
        decfsz  bit_count,F
        goto    flag_twist
        incf    next_freq,F
        call    tx_one_bit              ;send 0
        decfsz  flag_count,F
        goto    tx_flags
        return                          ;end of flags
;
;------------------------------------------------------------
;	Regs used:	AX25_crc_lo, AX25_crc_hi
;       Assumes data bit is in Carry
crc_core
        movlw   1
        skpnc
        xorwf   AX25_crc_lo, F          ; crc calculation
        clrc
        rrf     AX25_crc_hi, F
        rrf     AX25_crc_lo, F
        skpc
        return
        movlw   H'08'
        xorwf   AX25_crc_lo, F
        movlw   H'84'
        xorwf   AX25_crc_hi, F
        return
;
;------------------------------------------------------------
;	Regs used:	next_freq, FLAGS
tx_one_bit                              ;832 uS delay
        btfss   PIR1,TMR1IF             ;TMR1 timed out?
        goto    tx_one_bit              ;no
        movlw   high BIT_T              ;yes, reload
	movwf	TMR1H
	movlw	low BIT_T
	movwf	TMR1L
        bcf     PIR1,TMR1IF             ;clear flag
        btfss   next_freq,0             ;next_freq to W
        goto    lo_tone                 ;1, goto lo_tone
        bsf     TONE                    ;0, send 2200 Hz
        return
lo_tone
        bcf     TONE                    ;send 1200 Hz
        return
;------------------------------------------------------------
ee_write
	bsf	CD			;CD LED on
	movlw	EEADR
	movwf	FSR
	movfw	ee_addr
	movwf	INDF
	movlw	EEDATA
	movwf	FSR
	movfw	ee_data
	movwf	INDF
;
	bsf	STATUS,RP0
	bsf	EECON1,WREN
	bcf	INTCON,GIE		;ints off
	btfsc	INTCON,GIE
	goto	$-2
	movlw	0x55
	movwf	EECON2
	movlw	0xaa
	movwf	EECON2
	bsf	EECON1,WR
	btfsc	EECON1,WR
	goto	$-1
	bcf	EECON1,WREN
	bsf	INTCON,GIE		;ints on
	bcf	STATUS,RP0
	bcf	CD			;CD LED off
	return
ee_read
	movlw	EEADR
	movwf	FSR
	movfw	ee_addr
	movwf	INDF
	bsf	STATUS,RP0
	bsf	EECON1,RD
	bcf	STATUS,RP0
	movlw	EEDATA
	movwf	FSR
	movfw	INDF
	movwf	ee_data	
	return
;------------------------------------------------------------
check_for_CD
	bcf	CD			;CD LED off
	movfw	PORTB			;PORTB to W
	andlw	CD_MASK			;mask all but CD
	xorwf	last_look,W		;change?
	skpz				;no
	bsf	CD			;yes, CD LED on
	xorwf	last_look,F		;save copy
	return
;============================================================
;
;	USER DATA ENTRY ROUTINE AREA
;------------------------------------------------------------
;	Interactive menu collects path(s), TXD and echo state.
;	Regs Used:
;	count, loop_count, next_freq, rx_byte, temp,
;	ee_addr, ee_data, FSR, INDF, tx_buffer
;
;	Each callsign and optional SSID is processed as entered.
;	Error resets and displays query again.
;	Data is echoed back to terminal, backspace edit enabled.
;------------------------------------------------------------
;
tocall_area
	clrf	FLAGS
        clrf    loop_count
get_tocall_loop
        movfw   loop_count
        call    pwrup_msg		;send power on msg
	movwf	next_freq
	xorlw	d'0'
	bz	get_tocall		;done
	movfw	next_freq
        movwf   tx_buffer               ;load to send
	call	serial_tx
        incf    loop_count,F
        goto    get_tocall_loop
get_tocall
	call 	get_response
	btfsc	ERR_FLAG
	goto	tocall_area		;error, start over
	call	call_prep_store
	movlw	0x01
	movwf	offset
	call	xfer_to_eeprom		;store call in EEmem
;------------------------------------------------------------
mycall_area
	bcf	ERR_FLAG
        clrf    loop_count
mycall_loop
        movfw   loop_count
        call    mycall_table		;send  msg
	movwf	next_freq
	xorlw	d'0'
	bz	get_mycall		;done
	movfw	next_freq
        movwf   tx_buffer               ;load to send
	call	serial_tx
        incf    loop_count,F
        goto    mycall_loop
get_mycall
	call 	get_response
	btfsc	ERR_FLAG
	goto	mycall_area		;error, start over
	call	call_prep_store
	movlw	0x08
	movwf	offset
	call	xfer_to_eeprom		;store call in EEmem
;------------------------------------------------------------
via1_area
	bcf	ERR_FLAG
        clrf    loop_count
via1_loop
        movfw   loop_count
        call    via_1_table		;send  msg
	movwf	next_freq
	xorlw	d'0'
	bz	get_via1		;done
	movfw	next_freq
        movwf   tx_buffer               ;load to send
	call	serial_tx
        incf    loop_count,F
        goto    via1_loop
get_via1
	call 	get_response
	btfsc	BT_FLAG
	goto	txdelay_area		;no more calls
	btfsc	ERR_FLAG
	goto	via1_area		;error, start over
	call	call_prep_store	
	movlw	0x0f
	movwf	offset
	call	xfer_to_eeprom		;store call in EEmem
;------------------------------------------------------------
via2_area
	bcf	ERR_FLAG
        clrf    loop_count
via2_loop
        movfw   loop_count
        call    via_2_table		;send  msg
	movwf	next_freq
	xorlw	d'0'
	bz	get_via2		;done
	movfw	next_freq
        movwf   tx_buffer               ;load to send
	call	serial_tx
        incf    loop_count,F
        goto    via2_loop
get_via2
	call 	get_response
	btfsc	BT_FLAG
	goto	txdelay_area		;no more calls
	btfsc	ERR_FLAG
	goto	via2_area		;error, start over
	call	call_prep_store
	movlw	0x16
	movwf	offset
	call	xfer_to_eeprom		;store call in EEmem
;------------------------------------------------------------
via3_area
	bcf	ERR_FLAG
        clrf    loop_count
via3_loop
        movfw   loop_count
        call    via_3_table		;send  msg
	movwf	next_freq
	xorlw	d'0'
	bz	get_via3		;done
	movfw	next_freq
        movwf   tx_buffer               ;load to send
	call	serial_tx
        incf    loop_count,F
        goto    via3_loop
get_via3
	call 	get_response
	btfsc	BT_FLAG
	goto	txdelay_area		;no more calls
	btfsc	ERR_FLAG
	goto	via3_area		;error, start over
	call	call_prep_store
	movlw	0x1d
	movwf	offset
	call	xfer_to_eeprom		;store call in EEmem
;------------------------------------------------------------
txdelay_area
	clrf	FLAGS
        clrf    loop_count
txdelay_loop
        movfw   loop_count
        call    txd_table		;send  msg
	movwf	next_freq
	xorlw	d'0'
	bz	get_txdelay		;done
	movfw	next_freq
        movwf   tx_buffer               ;load to send
	call	serial_tx
        incf    loop_count,F
        goto    txdelay_loop
get_txdelay
	call 	get_response
	btfsc	ERR_FLAG
	goto	txdelay_area		;error, start over
	btfsc	BT_FLAG
	goto	txdelay_area		;no data entered
	clrf	temp
	movlw	RAML_START + 2
	xorwf	FSR,W
	bz	two_digit_txd
	movlw	RAML_START		
	movwf	FSR
	movfw	INDF
	sublw	0x39
	bnc	txdelay_area		;INDF > 9 ascii
	movlw	0x30
	subwf	INDF,W
	bnc	txdelay_area		;INDF < 0 ascii
	movfw	INDF			;single digit entry
	andlw	0x0f
	movwf	temp
	goto	store_txd_eemem
;
two_digit_txd
	decf	FSR,F
	movfw	INDF
	sublw	0x39
	bnc	txdelay_area		;INDF > 9 ascii
	movlw	0x30
	subwf	INDF,W
	bnc	txdelay_area		;INDF < 0 ascii
	movfw	INDF			;single digit entry
	andlw	0x0f
	movwf	temp			;store 1's in temp
	movlw	RAML_START		
	movwf	FSR
	movfw	INDF			;10's digit
	sublw	0x39
	bnc	txdelay_area		;INDF > 9 ascii
	movlw	0x30
	subwf	INDF,W
	bnc	txdelay_area		;INDF < 0 ascii
	movfw	INDF			;single digit entry
	andlw	0x0f
	movwf	loop_count		;
	movlw	.10
	addwf	temp,F			;add 10 each loop
	decfsz	loop_count,F
	goto	$-2
;
store_txd_eemem
	btfsc	AH_FLAG
	goto	txdelay_area		;saw error, try again
	movlw	TXD_EE
	movwf	ee_addr
	movfw	temp			;hex txd
	movwf	ee_data
	call	ee_write
;
;------------------------------------------------------------
echotext_area
	clrf	FLAGS
        clrf    loop_count
echotext_loop
        movfw   loop_count
        call    echo_table		;send  msg
	movwf	next_freq
	xorlw	d'0'
	bz	get_echotext		;done
	movfw	next_freq
        movwf   tx_buffer               ;load to send
	call	serial_tx
        incf    loop_count,F
        goto    echotext_loop
get_echotext
	call	serial_rx
	movfw	rx_byte
	movwf	tx_buffer
	call	serial_tx		;echo entry
	nop
  	movf  	tx_count,F              ;wait for done flag
   	bnz	$-2
	movlw	ECHO_EE
	movwf	ee_addr
	movfw	rx_byte			;hex txd
	movwf	ee_data
	call	ee_write
;
        clrf    loop_count
entry_done_loop
        movfw   loop_count
        call    entry_done_table	;send  msg
	movwf	next_freq
	xorlw	d'0'
	bz	$-1			;done, loop forever
	movfw	next_freq
        movwf   tx_buffer               ;load to send
	call	serial_tx		;send
        incf    loop_count,F
        goto    entry_done_loop
;	
;------------------------------------------------------------
get_response
	movlw	.9
	movwf	temp
	movlw	RAML_START + 9		;clear the temp storage
	movwf	FSR
	clrf	INDF
	decf	FSR,F
	decfsz	temp,F
	goto	$-3
response_loop
	call	serial_rx
	movlw	CR
	xorwf	rx_byte,W		;is it a CR? 
	bz	response_end		;yes, next step
	movlw	BACKSPACE		;no, check for backspace
	xorwf	rx_byte,W		;backspace?
	bnz	store_this_entry	;no, do store routine
	movlw	RAML_START		;yes, check if at start
	xorwf	FSR,W
	bz	response_loop		;at start, do nothing
	decf	FSR,F			;prep to overwrite
	movlw	BACKSPACE
	movwf	tx_buffer
	call	serial_tx
	movlw	" "
	movwf	tx_buffer
	call	serial_tx
	movlw	BACKSPACE
	movwf	tx_buffer
	call	serial_tx
	goto	response_loop
store_this_entry
	movfw	rx_byte
	movwf	INDF
	incf	FSR,F			;inc loc for next store
echo_back
	movfw	rx_byte
        movwf   tx_buffer               ;load to send
	call	serial_tx
	goto	response_loop
response_end
	movlw	RAML_START
	xorwf	FSR,W			;blow thru?
	bz	blow_thru
	movfw	rx_byte
	movwf	INDF			;store <CR>
	movlw	RAML_START + 9		;CALLxx-Xx
	subwf	FSR,W
	bnc	$+2
	bsf	ERR_FLAG
	return
blow_thru
	bsf	BT_FLAG
	return
;
;------------------------------------------------------------
;	Uses scratch pad RAM preloaded with 6 spaces and 30h 
;	SSID. Calls less than 6 chars are padded w/spaces.
;	1 or 2 decimal SSID is converted to 4 bit hex.
;	Each callsign entered is processed with this routine.	
;
call_prep_store
	movlw	6
	movwf	offset
	movlw	SCRATCH_PAD
	movwf	FSR
prep_scratch_loop
	movlw	" "
	movwf	INDF			;6 spaces
	incf	FSR,F
	decfsz	offset,F
	goto	prep_scratch_loop
	movlw	b'00110000'		;set reserved bits
	movwf	INDF			;7th byte 0
;	
scan_call
	movlw	RAML_START
	addwf	offset,W
	movwf	FSR			;RAML_START + offset
	movlw	"-"
	xorwf	INDF,W			;is it a hyphen?
	bz	conv_call_ssid		;yes
	movlw	0x30			;no		
	subwf	INDF,W			;is it non-data?
	bnc	ssid_is_zero		;yes
	movlw	6			;no, do we have 6 chars?
	xorwf	offset,W
	bz	ssid_is_zero		;yes, and no SSID
	movfw	INDF			;
	movwf	temp			;move data to scratchpad
	movlw	SCRATCH_PAD
	addwf	offset,W
	movwf	FSR			;store byte here
	movfw	temp
	movwf	INDF			;overwrite space
	incf	offset,F
	goto	scan_call
;
conv_call_ssid
	movlw	2	
	addwf	FSR,F	
	movlw	0x30
	subwf	INDF,W			;test for <CR/LF>
	bnc	one_digit_ssid		;
	movfw	INDF	
	addlw	.10			;add 10
	movwf	temp
	goto	temp_store_ssid	
one_digit_ssid
	decf	FSR,F
	movfw	INDF
	movwf	temp			;store ssid
temp_store_ssid
	movlw	SCRATCH_PAD + 6
	movwf	FSR
	movfw	temp
	andlw	0x0f			;4 bit number
	iorlw	b'00110000'		;set reserved bits
	movwf	INDF			;overwrite zero	
;	
ssid_is_zero		
	return				;
;------------------------------------------------------------
;	offset contains EEmem starting loc for each callsign
;
xfer_to_eeprom
	movlw	7
	movwf	loop_count		;xfer 7 bytes
	clrf	count
xfer_to_eeprom_loop
	movlw	SCRATCH_PAD
	addwf	count,W
	movwf	FSR			;indirect data loc
	movfw	INDF
	movwf	ee_data			;byte to write
	movfw	offset			;where to put it
	movwf	ee_addr			;set EE address
	call	ee_write		;write data to EEMEM
	incf	count,F			;inc pointers
	incf	offset,F
	decfsz	loop_count,F		;dec loop counter
	goto	xfer_to_eeprom_loop
	movfw	ee_addr
	movwf	ee_data
	clrf 	ee_addr
	call	ee_write		;store last byte loc at 0	
	return
;
;------------------------------------------------------------
;	Transfers EE call/path data to bank 2 hi memory 
;	Data is preconfigured (left shifted) for transmit
;	1st EE byte is eof address
;	0x00 is eof in hi memory
;
load_stored_path
	clrf	loop_count		;offset
	clrf	ee_addr			;reset eeaddress	
	call	ee_read			;get eedata
	movfw	ee_data
        movwf	count			;1st byte is eof loc
	sublw	d'13'
	bc	no_path_stored		;invalid 
	movfw	ee_data
	sublw	d'35'
	bnc	no_path_stored		;invalid
	incf	ee_addr,F		;point to second byte
stored_path_loop
	call	ee_read			;get eedata
	movfw	ee_data
        movwf   temp                    ;copy to temp
        clrc                            ;clear carry
        rlf     temp,F                  ;shift left
	movlw	BANK2_START		;hi mem data start
	addwf	loop_count,W
	movwf	FSR			;reload correct FSR
        movfw   temp                    ;reload W
;
	bsf	STATUS,IRP		;bank 2,3
	movwf	INDF			;store W at indirect offset
	bcf	STATUS,IRP		;bank 0,1
;
	incf	loop_count,F
	incf	ee_addr,F
	movfw	count
	xorwf	ee_addr,W
	bz	end_of_path
        goto    stored_path_loop        ;get next
;
;------------------------------------------------------------
end_of_path
	call	ee_read			;get eedata
	movfw	ee_data
        movwf   temp                    ;copy to temp					
        setc                            ;set carry
        rlf     temp,F                  ;shift left
	movlw	BANK2_START		;reload correct FSR
	addwf	loop_count,W
	movwf	FSR
        movfw   temp                    ;reload W (last SSID)
;
	bsf	STATUS,IRP		;bank 2,3
	movwf	INDF			;store W at indirect offset
	bcf	STATUS,IRP		;bank 0,1
;
	incf	FSR,F
        movlw	0x03                    ;0x03
	bsf	STATUS,IRP		;bank 2,3
	movwf	INDF			;store W at indirect offset
	bcf	STATUS,IRP		;bank 0,1
	incf	FSR,F
        movlw	0xf0                    ;0xf0
	bsf	STATUS,IRP		;bank 2,3
	movwf	INDF			;store W at indirect offset
	bcf	STATUS,IRP		;bank 0,1
	incf	FSR,F
        movlw	0	                ;0 - eof flag
	bsf	STATUS,IRP		;bank 2,3
	movwf	INDF			;store W at indirect offset
	bcf	STATUS,IRP		;bank 0,1
;
transmit_delay_read
	movlw	TXD_EE			;txd loc in eemem
	movwf	ee_addr
	call	ee_read			;get eedata
	movfw	ee_data
	andlw	b'01111111'		;mask bit 7
	movwf	txd_count		;holdoff loaded
echo_setting_read
	movlw	ECHO_EE
	movwf	ee_addr
	call	ee_read			;get eedata
	movlw	"Y"
	xorwf	ee_data,W
	skpnz
	bsf	txd_count,7		;txd_count bit 7 set
	return	
;
;------------------------------------------------------------
no_path_stored
	bsf	CD
	nop
	goto	$-1
;------------------------------------------------------------
        end
;------------------------------------------------------------
;
Copyright (c) 2004, Mike Berg aka N0QBH
All rights reserved.
