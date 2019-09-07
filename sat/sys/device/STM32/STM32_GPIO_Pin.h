
#ifndef _QB50_SYS_DEVICE_STM32_GPIO_PIN_H
#define _QB50_SYS_DEVICE_STM32_GPIO_PIN_H

#ifndef _QB50_SYS_DEVICE_STM32_GPIO_H
 #error "can't include STM32_GPIO_Pin.h, try STM32_GPIO.h instead"
#endif


class Pin : public GPIO::Pin
{

   public:

      Pin( STM32_GPIO& gpio, const unsigned id, const char *name, const uint16_t mask );
      ~Pin();

      Pin& on     ( void );
      Pin& off    ( void );
      Pin& toggle ( void );
      bool read   ( void );

      unsigned      id ( void ) const { return _id; }
      unsigned  portId ( void ) const { return _gpio.id(); }

      Pin& mode   ( Mode mode );

      Pin& in     ( void ) { return mode( INPUT  ); }
      Pin& out    ( void ) { return mode( OUTPUT ); }
      Pin& analog ( void ) { return mode( ANALOG ); }

      Pin& alt    (      Alt alt   );
      Pin& oSpeed (   OSpeed speed );
      Pin& oType  (    OType type  );
      Pin& PuPd   ( PullUpDn pud   );

      Pin& noPull ( void ) { return PuPd( NOPULL ); }
      Pin& pullUp ( void ) { return PuPd( PULLUP ); }
      Pin& pullDn ( void ) { return PuPd( PULLDN ); }

      Pin& setHandler ( Handler *handler, Edge edge );

   protected:

      STM32_GPIO&     _gpio;
      const unsigned  _id;
      const uint16_t  _mask;

};

#endif /* _QB50_SYS_DEVICE_STM32_GPIO_PIN_H */

/*EoF*/
