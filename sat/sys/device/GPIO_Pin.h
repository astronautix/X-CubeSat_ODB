
#ifndef _QB50_SYS_DEVICE_GPIO_PIN_H
#define _QB50_SYS_DEVICE_GPIO_PIN_H

#ifndef _QB50_SYS_DEVICE_GPIO_H
 #error "can't include GPIO_Pin.h, try GPIO.h instead"
#endif


class Pin
{

   public:

      enum Edge {
         RISING  = 0,
         FALLING = 1,
         BOTH    = 2
      };

      Pin( const char *name ) : _name( name )
      { ; }

      virtual ~Pin()
      { ; }

      inline const char *name ( void ) const
      { return _name; }

      virtual Pin& in     ( void ) = 0;
      virtual Pin& out    ( void ) = 0;
      virtual Pin& on     ( void ) = 0;
      virtual Pin& off    ( void ) = 0;
      virtual Pin& toggle ( void ) = 0;
      virtual bool read   ( void ) = 0;

      /* input signal handler */

      virtual Pin& setHandler ( Handler *handler, Edge edge ) = 0;

   protected:

      const char *_name;

};

#endif /* _QB50_SYS_DEVICE_GPIO_PIN_H */

/*EoF*/
