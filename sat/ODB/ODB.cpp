
#include "devices.h"
#include "AX25Modem.h"
#include "Baseband.h"
#include "CTCSS.h"
#include "ADCS/ADCS.h"
#include "FiPEX/Fipex.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

ODB::ODB( const char *name,
          STM32_GPIO::Pin& selPin,
          STM32_GPIO::Pin& inhPin,
          STM32_GPIO::Pin& asPin,
          STM32_GPIO::Pin& adPin )
   : _name( name ),
     _selPin( selPin ),
     _inhPin( inhPin ),
     _asPin( asPin ),
     _adPin( adPin )
{
   maI[0] = maI[1] = maI[2] = maI[3] = 0;
   mvV[0] = mvV[1] = mvV[2] = mvV[3] = 0;
   maIRx  = 0;
   maITx  = 0;
   mvBat  = 0;
   dcBat  = 0;
}


ODB::~ODB()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

ODB& ODB::init( void )
{
   SYSLOG.init();
      RCC.init();
      WDG.init();
      PWR.init();
      BKP.init();
     RTC0.init();
     NVIC.init();
     EXTI.init();
     DMA1.init();
     DMA2.init();
    GPIOA.init().enable();
    GPIOB.init().enable();
    GPIOC.init().enable();
    UART1.init();
    UART2.init();
    UART6.init();
     SPI3.init();
     ADC1.init();
     ADC2.init();
     ADC3.init();
     ADC4.init();
 //VFLASH.init();
     FCH0.init();
     FCH1.init();
     WOD0.init();
     WOD1.init();
    //WDB.init();
       BB.init();
     M1K2.init();
     M9K6.init();
    CTCS0.init();
    ADCS0.init();
       SU.init();

   _selPin.in().noPull();
   _inhPin.in().pullUp();
   _asPin.in().noPull();
   _adPin.out().off();

   kprintf( "%s: AMSAT-F micro-satellite\r\n", _name );

   return *this;
}


ODB::SatSel ODB::id( void )
{
   return
      _selPin.read() ? ODB::FR01 : ODB::FR05;
}


bool ODB::isInhibit( void )
{
   return
      ( _inhPin.read() == 0 );
}


ODB::AntState ODB::aState( void )
{
   if( isInhibit() ) {
      kprintf( "%s: Inhibit ON\r\n", _name );
      return ODB::DEPLOYED;
   }

   return
      _asPin.read() ? ODB::DEPLOYED : ODB::RETRACTED;
}


ODB::AntState ODB::aDeploy( void )
{
   ODB::AntState st = ODB::RETRACTED;

   if( aState() == ODB::DEPLOYED ) {
      return ODB::DEPLOYED;
   }

   _adPin.on();

   for( int i = 0 ; i < 3 ; ++i )
   {
      kprintf( "Trying to deploy antenna (%d/3)...\r\n", ( i + 1 ));
      delay( 4999 );

      st = aState();
      if( st == ODB::DEPLOYED )
         break;
   }

   _adPin.off();

   if( st == ODB::DEPLOYED )
      kprintf( "Antenna deployed\r\n" );
   else
      kprintf( "Giving up on antenna deployment\r\n" );

   return st;
}

/*EoF*/
