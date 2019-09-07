
#ifndef _QB50_SYS_DEVICE_STM32_DMA_STREAM_H
#define _QB50_SYS_DEVICE_STM32_DMA_STREAM_H

#ifndef _QB50_SYS_DEVICE_STM32_DMA_H
 #error "can't include STM32_DMA_Stream.h, try STM32_DMA.h instead"
#endif


class Stream : public Device
{

   public:

      Stream( STM32_DMA& dma, const uint32_t iobase, const uint32_t IRQn, const char *name, const uint32_t shl );
      ~Stream();

      Stream& init    ( void );
      Stream& enable  ( bool debug = false );
      Stream& disable ( bool debug = false );

      Stream& start   ( void );
      Stream& stop    ( void );
      Stream& wait    ( void );

      inline Stream& channel( Channel sel )
      { return _updateCR( sel, 0x07, 25 ); }

      inline Stream& mBurst( Burst sel )
      { return _updateCR( sel, 0x03, 23 ); }

      inline Stream& pBurst( Burst sel )
      { return _updateCR( sel, 0x03, 21 ); }

      inline Stream& target( Target sel )
      { return _updateCR( sel, 0x01, 19 ); }

      inline Stream& priority( Priority sel )
      { return _updateCR( sel, 0x03, 16 ); }

      inline Stream& mDataSize( DataSize sel )
      { return _updateCR( sel, 0x03, 13 ); }

      inline Stream& pDataSize( DataSize sel )
      { return _updateCR( sel, 0x03, 11 ); }

      inline Stream& mIncMode( IncMode sel )
      { return _updateCR( sel, 0x01, 10 ); }

      inline Stream& pIncMode( IncMode sel )
      { return _updateCR( sel, 0x01, 9 ); }

      inline Stream& mode( Mode sel )
      { return _updateCR( sel, 0x01, 8 ); }

      inline Stream& direction( Direction sel )
      { return _updateCR( sel, 0x03, 6 ); }

      Stream& counter ( uint16_t cnt  );
      Stream& pAddr   ( uint32_t addr );
      Stream& m0Addr  ( uint32_t addr );
      Stream& m1Addr  ( uint32_t addr );

      void isr( void );

   private:

      Stream& _updateCR ( uint32_t val, uint32_t mask, int shift );
      Stream& _clearIFR ( uint32_t flags );

      xSemaphoreHandle _isrTxIE;  /**< ISR semaphore bound to TCIE and TEIE */

      STM32_DMA&     _dma;
      const uint32_t _iobase;
      const uint32_t _IRQn;
      const uint32_t _shl;

};


#endif /*_QB50_SYS_DEVICE_STM32_DMA_STREAM_H*/

/*EoF*/
