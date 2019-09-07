
#ifndef _QB50_SYS_DEVICE_STM32_TIMER_CHANNEL_H
#define _QB50_SYS_DEVICE_STM32_TIMER_CHANNEL_H

#ifndef _QB50_SYS_DEVICE_STM32_TIMER_H
 #error "can't include STM32_Timer_Channel.h, try STM32_Timer.h instead"
#endif


class Channel : public Device
{

   public:

      Channel( STM32_Timer &tmr, const char *name, ChId id );
      virtual ~Channel();

      Channel& init    ( void );
      Channel& enable  ( bool debug = false );
      Channel& disable ( bool debug = false );


   private:

      STM32_Timer &_tmr;  // timer
      ChId         _cid;  // channel id

};


#endif /*_QB50_SYS_DEVICE_STM32_TIMER_CHANNEL_H*/

/*EoF*/
