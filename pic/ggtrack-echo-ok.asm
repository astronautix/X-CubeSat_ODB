
   radix       dec
   list        p=16F628
   errorlevel  -302

   include     <p16f628.inc>

   __CONFIG    0x3f62


;;  - - - - - - - - -  ;;
;;  C O N S T A N T S  ;;
;;  - - - - - - - - -  ;;

   ; modes

   cblock 0x00

      m_IDLE      ; 0x00 - IDLE mode
      m_SERIAL    ; 0x01 - SERIAL mode

   endc


   ; global registers

   cblock 0x70

      r_WSAVE     ; 0x70 - (ISR) backup for W
      r_SSAVE     ; 0x71 - (ISR) backup for STATUS
      r_PSAVE     ; 0x72 - (ISR) backup for PCLATH

      r_mode      ; 0x73 - current mode
      r_rxbc      ; 0x74 - rx bit count
      r_rxsc      ; 0x75 - rx sample count
      r_rx1c      ; 0x76 - rx 'logic high' sample count
      r_rxByte    ; 0x77 - rx byte

   endc


;;  - - - - - -  ;;
;;  M A C R O S  ;;
;;  - - - - - -  ;;

m_ISREnter macro

   ; save W, STATUS and PCLATH registers

   movwf    r_WSAVE
   swapf    STATUS, W
   clrf     STATUS
   movwf    r_SSAVE
   movf     PCLATH, W
   clrf     PCLATH
   movwf    r_PSAVE

   endm


m_ISRLeave macro

   ; restore W, STATUS and PCLATH registers

   movf     r_PSAVE, W
   movwf    PCLATH
   clrf     STATUS
   swapf    r_SSAVE, W
   movwf    STATUS
   swapf    r_WSAVE, F
   swapf    r_WSAVE, W

   endm


;;  - - - - - - - - - - - -  ;;
;;  R E S E T   V E C T O R  ;;
;;  - - - - - - - - - - - -  ;;

   org      0x0000

   goto     _main


;;  - - - - - - - - - - -  ;;
;;  I S R   H A N D L E R  ;;
;;  - - - - - - - - - - -  ;;

   org      0x0004

   m_ISREnter

   ; [debug] enable PA1

   banksel  PORTA
   bsf      PORTA, RA1

   ; clear TMR2IF flag

   banksel  PIR1 
   bcf      PIR1, TMR2IF

   ; figure out what to do based on r_mode

   movf     r_mode, W
  ;andlw    0x03
   addwf    PCL, F

_ISRSwitch:

   goto     _IDLE       ; r_mode == 0 - handle IDLE mode
   goto     _SERIAL     ; r_mode == 1 - handle SERIAL mode
   goto     _ISREnd     ; r_mode == 2 - (not implemented)
   goto     _ISREnd     ; r_mode == 3 - (not implemented)


_IDLE:

   call     _toSERIAL
   goto     _ISREnd


_SERIAL:

   ;; this code gets called 38400 times/sec (4 * 9600Hz),
   ;; -> that's 4 samples per bit on the serial line.
   ;;
   ;; . read RA4 (one sample)
   ;; . if RA4 == 1, increment rx1c ("high" sample count)
   ;; . decrement rxsc (bit sample count)
   ;; . when rxsc reaches 0 (4 samples = one bit):
   ;;   . if rx1c > 2, shift 1 into rxByte, otherwise shift 0
   ;;   . decrement rxbc (bit count)
   ;;   . stop after rxbc reaches 0 (got 9 bits)
   ;;
   ;; the start bit gets shifted through rxByte just like
   ;; any other data bit;  since rxByte is 8 bits wide, the
   ;; start bit will pop out to the carry flag at the end,
   ;; leaving us with our 8-bit value in rxByte.

   banksel  PORTA
   btfss    PORTA, RA4
   goto     _loSample

_hiSample:

   ; "high" sample (logic level 1)

   incf     r_rx1c, F
; bsf      PORTA, 3
; goto     _xxx

_loSample:

   ; "low" sample (logic level 0)

; bcf      PORTA, 3
;_xxx:
   decfsz   r_rxsc, F
   goto     _rxDone

   ; got 4 samples

   movf     r_rx1c, W
   sublw    2           ; c = (( 2 - rx1c ) >= 0 )
   rrf      r_rxByte, W
   xorlw    0x80
   movwf    r_rxByte

;

   banksel  PORTA
   btfss    r_rxByte, 7
   bcf      PORTA, 3
   btfsc    r_rxByte, 7
   bsf      PORTA, 3

;

   decfsz   r_rxbc, F
   goto     _nextBits

   ; got start bit + 8 data bits
   ; switch back to IDLE mode

   call     _toIDLE
   goto     _rxDone

_nextBits:

   ; more bits to read

   movlw    4
   movwf    r_rxsc
   clrf     r_rx1c

_rxDone:


_ISREnd:

   ; [debug] disable PA1

   banksel  PORTA
   bcf      PORTA, RA1

   m_ISRLeave

   retfie


;;  - - - - - - - - - - - - - - - -  ;;
;;  M O D E S   M A N A G E M E N T  ;;
;;  - - - - - - - - - - - - - - - -  ;;

_toIDLE:

 banksel  PORTA
 bsf      PORTA, RA0
   ; disable TMR2

   banksel  PIR1 
   bcf      PIR1,  TMR2IF  ; clear TMR2 interrupt flag
   bcf      T2CON, TMR2ON  ; disable TMR2 interrupt

   ; get TMR0 ready to overflow on the first event
   ; (falling edge on RA4)

   movlw    0xff
   movwf    TMR0

   ; set new mode to m_IDLE

   movlw    m_IDLE
   movwf    r_mode

   ; enable TMR0

   bcf      INTCON, T0IF   ; clear TMR0 interrupt flag
   bsf      INTCON, T0IE   ; enable TMR0 interrupt

   return


_toSERIAL:

 banksel  PORTA
 bcf      PORTA, RA0
   ; disable TMR0

   bcf      INTCON, T0IF   ; clear TMR0 interrupt flag
   bcf      INTCON, T0IE   ; disable TMR0 interrupt

   ; setup TMR2

;
; . repasser PR2 à 63
; . utiliser TMR1 pour générer le 9600 du TX (txByte)
;   -> TMR1 tourne en permanence
;


   movlw    62             ; TMR2 period
   banksel  PR2
   movwf    PR2

   movlw    48             ; TMR2 module
   banksel  TMR2
   movwf    TMR2

   ; rxsc = 3

   movlw    3
   movwf    r_rxsc

   ; rx1c = 0

   clrf     r_rx1c

   ; rxbc = 9

   movlw    10 ; XXX 9 !
   movwf    r_rxbc

   ; rxByte = 0

   clrf     r_rxByte

   ; set new mode to m_SERIAL

   movlw    m_SERIAL
   movwf    r_mode

   ; enable TMR2

   banksel  PIR1 
   bcf      PIR1,  TMR2IF  ; clear TMR2 interrupt flag
   bsf      T2CON, TMR2ON  ; enable TMR2 interrupt

   return


;;  - - - -  ;;
;;  M A I N  ;;
;;  - - - -  ;;

_main:

   ;;
   ;; initialization
   ;;

   ; clear status flags, switch to bank #0

   clrf     STATUS

   ; disable comparators

   movlw    0x07
   movwf    CMCON

   ; disable interrupts, clear all pending interrupt flags

   clrf     INTCON

   ; configure the I/O ports

   movlw    0xf0
   banksel  TRISA
   movwf    TRISA
   movwf    TRISB

   banksel  PORTA
   clrf     PORTA
   clrf     PORTB

   bsf      PORTA, RA3

   ; setup TMR0

   banksel  OPTION_REG
   bsf      OPTION_REG, PSA
   bsf      OPTION_REG, T0SE
   bsf      OPTION_REG, T0CS

   ; setup TMR2

   banksel  T2CON
   clrf     T2CON          ; set prescaler & postscaler to 1:1

   banksel  PIE1
   bsf      PIE1, TMR2IE

   ; enable interrupts

   bsf      INTCON, GIE    ; enable global interrupts
   bsf      INTCON, PEIE   ; enable peripheral interrupts

   ;;
   ;; main loop
   ;;

   ; switch to IDLE mode

   call     _toIDLE

_loop:

   nop

   goto     _loop

   end

