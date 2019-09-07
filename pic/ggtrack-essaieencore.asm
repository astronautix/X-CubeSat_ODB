
   radix       dec
   list        p=16F628
   errorlevel  -302

   include     <p16f628.inc>

   __CONFIG    0x3f62


#define FOSC  10000000
#define F9600 (( FOSC / 4 / 9600 / 2 ) - 1 )


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

      r_flags     ; 0x73 - flags (current mode, rxByte/txByte ready, etc..)

      r_rxbc      ; 0x74 - rx bit count
      r_rxTmp     ; 0x75 - rx byte being received
      r_rxByte    ; 0x76 - last received byte

      r_txbc      ; 0x77 - tx bit count
      r_txTmp     ; 0x78 - tx byte being transmitted
      r_txByte    ; 0x79 - next byte to transmit

   endc

   ; flags

   cblock 0

      F_RXRDY     ; r_flags[0] - new byte received in rxByte
      F_TXRDY     ; r_flags[1] - ready to transmit from txByte
      F_9600      ; r_flags[2] - 1200/9600 baud

   endc


;;  - - - - - -  ;;
;;  M A C R O S  ;;
;;  - - - - - -  ;;

mISREnter macro

   ; save W, STATUS and PCLATH registers

   movwf    r_WSAVE        ; +1  1
   swapf    STATUS, W      ; +1  2
   clrf     STATUS         ; +1  3
   movwf    r_SSAVE        ; +1  4
   movf     PCLATH, W      ; +1  5
   clrf     PCLATH         ; +1  6
   movwf    r_PSAVE        ; +1  7

   endm


mISRLeave macro

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

   org      0x0004         ;     0

   mISREnter               ; +7  7

   ; [debug] enable RA1

   banksel  PORTA          ; +2  9
   bsf      PORTA, RA1     ; +1 10


_handleTMR1:

   ; TMR1IF is set: an outgoing serial transmission is in progress
   ; -> handle this case first in order to reduce jitter on the TX line
   ;    (reason: no auro-reload on TMR1, we have to do it by hand)
   ; -> shift txTmp out to RA3

   banksel  PIR1           ; +2 12
   btfss    PIR1, TMR1IF   ; +1 13
   goto     _handleTMR0    ; +2 15

   ; reload TMR1 first, ask questions later...

   movlw    0x11           ; +1 16
   movwf    TMR1L          ; +1 17 instruction cycles into the ISR
   decf     TMR1H, F       ; TMR1H <- 0xff

   ; acknowledge the interrupt on TMR1

   bcf      PIR1, TMR1IF


_handleTMR0:

   ; T0IF is set: we just reveived a start bit
   ; -> get ready to receive a byte

   btfss    INTCON, T0IF
   goto     _handleTMR2

   ; acknowledge the interrupt, then disable TMR0

   bcf      INTCON, T0IF
   bcf      INTCON, T0IE

   call     _setupRX


_handleTMR2:

   ; TMR2IF is set: an incoming serial transmission is in progress
   ; -> shift RA4 into rxTmp

   banksel  PIR1 
   btfss    PIR1, TMR2IF
   goto     _ISREnd

   ; acknowledge the interrupt on TMR2

   bcf      PIR1, TMR2IF

   ; shift in a bit from RA4

   banksel  PORTA

   swapf    PORTA, W       ; copy RA4[7..4] to W[3..0]
   andlw    0x01           ; W is now either 0 or 1
   addlw    0xff           ; set the carry if W == 1
   rrf      r_rxTmp, f     ; shift in the carry to rxTmp

   btfsc    r_rxbc, 7
   goto     _stopBit

   decfsz   r_rxbc, F
   goto     _nextBits

   ; we have a full byte
   ; -> copy rxTmp over to rxByte
   ; -> prepare for the stop bit

   movf     r_rxTmp, W
   movwf    r_rxByte       ; move rxTmp over to rxByte
   bsf      r_rxbc, 7      ; expect a stop bit next time

   goto     _ISREnd

_stopBit:

   ; we have a stop bit
   ; -> switch back to IDLE mode
   ; -> set the "RX Ready" flag

   call     _toIDLE
   bsf      r_flags, F_RXRDY

_nextBits:

   ; more bits to read

_ISREnd:

   ; [debug] disable RA1

   banksel  PORTA
   bcf      PORTA, RA1

   mISRLeave

   retfie


;;  - - - - - - - - - - - - - - - -  ;;
;;  M O D E S   M A N A G E M E N T  ;;
;;  - - - - - - - - - - - - - - - -  ;;

_toIDLE:

   ; disable TMR2

   banksel  PIR1 
   bcf      PIR1,  TMR2IF  ; clear TMR2 interrupt flag
   bcf      T2CON, TMR2ON  ; disable TMR2 interrupt

   ; get TMR0 ready to overflow on the first event
   ; (falling edge on RA4)

   movlw    0xff
   movwf    TMR0

   ; enable TMR0

   bcf      INTCON, T0IF   ; clear TMR0 interrupt flag
   bsf      INTCON, T0IE   ; enable TMR0 interrupt

   return


_setupRX:

   ; setup TMR2

   movlw    F9600          ; TMR2 period
   banksel  PR2
   movwf    PR2

   banksel  TMR2
   clrf     TMR2

   ; prepare to read 8 bits

   movlw    8
   movwf    r_rxbc

   ; enable TMR2

   banksel  PIR1 
   bcf      PIR1,  TMR2IF  ; clear TMR2 interrupt flag
  ;bsf      T2CON, TMR2ON  ; enable TMR2 interrupt

   movlw    0x0c           ; set postscaler to 1:2 ...
   movwf    T2CON          ; ... and enable TMR2 interrupt

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

   ; setup TMR1

   banksel  T1CON
   clrf     T1CON
   movlw    0x1f
   movwf    TMR1H
   movlw    0x00
   movwf    TMR1L
   bsf      T1CON, TMR1ON

   banksel  PIE1
   bsf      PIE1, TMR1IE

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

   btfss    r_flags, F_RXRDY
   goto     _loop

   bcf      r_flags, F_RXRDY
   banksel  PORTA
   bsf      PORTA, RA0
   bcf      PORTA, RA0

   goto     _loop

   end

