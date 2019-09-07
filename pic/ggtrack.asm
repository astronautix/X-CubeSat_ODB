
   radix       dec
   list        p=16F628
   errorlevel  -302

   include     <p16f628.inc>

   __CONFIG    0x3f62


#define FOSC  10000000
#define F1200 (( FOSC / 4 / ( 18 * 1200 )) - 1 )
#define F2200 (( FOSC / 4 / ( 18 * 2200 )) - 1 )


;;  - - - - - - - - -  ;;
;;  C O N S T A N T S  ;;
;;  - - - - - - - - -  ;;

   ; global registers

   cblock 0x70

      r_WSAVE     ; 0x70 - (ISR) backup for W
      r_SSAVE     ; 0x71 - (ISR) backup for STATUS
      r_PSAVE     ; 0x72 - (ISR) backup for PCLATH

      r_CNT       ; 0x73 - TMR2 counter
      r_SAMPLE    ; 0x74 - sample #

   endc


;;  - - - - - -  ;;
;;  M A C R O S  ;;
;;  - - - - - -  ;;

mISREnter macro

   ; save W, STATUS and PCLATH registers

   movwf    r_WSAVE
   swapf    STATUS, W
   clrf     STATUS
   movwf    r_SSAVE
   movf     PCLATH, W
   clrf     PCLATH
   movwf    r_PSAVE

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

   org      0x0004

   mISREnter

   ; acknowledge TMR2 interrupt

   banksel  PIR1 
   bcf      PIR1, TMR2IF

   decfsz   r_CNT, F
   goto     _dds

   ; counter == 0: one bit period
   ; -> send a pulse on RA3
   ; -> read RA4, set CNT and PR2 accordingly

   banksel  PORTA
   bsf      PORTA, RA3

   btfsc    PORTA, RA4
   goto     _hiRA4

_loRA4:

   ; RA4 low: switch to 1200Hz

   movlw    18
   movwf    r_CNT
   movlw    F1200

   goto     _setPR2

_hiRA4:

   ; RA4 high: switch to 2200Hz

   movlw    33
   movwf    r_CNT
   movlw    F2200

_setPR2:

   banksel  PR2
   movwf    PR2

   banksel  RA3
   bcf      PORTA, RA3

_dds:

   ; generate a sample on RB[3..0] 

   call     _sinv
   banksel  PORTB
   movwf    PORTB

   mISRLeave

   retfie


;;  - - - - - - - - - -  ;;
;;  S I N E   T A B L E  ;;
;;  - - - - - - - - - -  ;;

_sinv:

   decfsz   r_SAMPLE, W
   goto     _sinv_lookup

   movlw    18

_sinv_lookup:

   movwf    r_SAMPLE 
   addwf    PCL, F

   nop
   retlw     5    ; .....*
   retlw     3    ; ...*
   retlw     1    ; .*
   retlw     0    ; *
   retlw     0    ; *
   retlw     1    ; .*
   retlw     3    ; ...*
   retlw     5    ; .....*
   retlw     8    ; ........*
   retlw    10    ; ..........*
   retlw    12    ; ............*
   retlw    14    ; ..............*
   retlw    15    ; ...............*
   retlw    15    ; ...............*
   retlw    14    ; ..............*
   retlw    12    ; ............*
   retlw    10    ; ..........*
   retlw     8    ; ........*


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

   banksel  TRISA
   movlw    0xf7           ; RA3 = IN, others = OUT
   movwf    TRISA
   movlw    0xb0           ; RB6 = IN, others = OUT
   movwf    TRISB

   ; enable pull-ups on PORTB inputs

   bsf      OPTION_REG, NOT_RBPU

   ; set outputs to logical low

   banksel  PORTA
   clrf     PORTA
   clrf     PORTB

   ; setup TMR2

   banksel  T2CON
   clrf     T2CON          ; set prescaler & postscaler to 1:1

   movlw    F1200
   banksel  PR2
   movwf    PR2

   movlw    18
   movwf    r_CNT
   movwf    r_SAMPLE

   banksel  TMR2
   clrf     TMR2

   banksel  PIE1
   bsf      PIE1, TMR2IE

   banksel  T2CON
   bsf      T2CON, TMR2ON  ; enable TMR2 interrupt

   ; enable interrupts

   bsf      INTCON, GIE    ; enable global interrupts
   bsf      INTCON, PEIE   ; enable peripheral interrupts

   ;;
   ;; main loop
   ;;

   goto     $

   end

