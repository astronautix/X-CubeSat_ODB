
#ifndef _QB50_SYS_DEVICE_GPIO_HANDLER_H
#define _QB50_SYS_DEVICE_GPIO_HANDLER_H

#ifndef _QB50_SYS_DEVICE_GPIO_H
 #error "can't include GPIO_Handler.h, try GPIO.h instead"
#endif


class Handler
{

   public:

      Handler()
      { ; }

      virtual ~Handler()
      { ; }

      virtual void handle( Pin& pin ) = 0;

};

#endif /* _QB50_SYS_DEVICE_GPIO_HANDLER_H */

/*EoF*/
