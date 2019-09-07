
#ifndef _QB50_SYS_DEVICE_ADC_CHANNEL_H
#define _QB50_SYS_DEVICE_ADC_CHANNEL_H

#ifndef _QB50_SYS_DEVICE_ADC_H
 #error "can't include ADC_Channel.h, try ADC.h instead"
#endif


class Channel : public Sensor<T>
{

   public:

      ADC& _adc;

      Channel( ADC& adc ) : _adc( adc )
      { ; }

      virtual ~Channel()
      { ; }


   protected:

      Channel& _readRaw( SensorSample<T> *sample )
      {
         (void)_adc.read( *this, sample );
         return *this;
      }

};


#endif /*_QB50_SYS_DEVICE_ADC_CHANNEL_H*/

/*EoF*/
