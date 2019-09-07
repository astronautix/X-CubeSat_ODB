
#include "config.h"

#include <safe_stm32f4xx.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#undef errno
extern int errno;


extern "C" {

 int     _close  ( int fd )                            __attribute__((used));
 void    _exit   ( int status )                        __attribute__((used));
 int     _fstat  ( int fd, struct stat *st )           __attribute__((used));
 int     _getpid ( void )                              __attribute__((used));
 int     _isatty ( int fd )                            __attribute__((used));
 int     _kill   ( int pid, int sig )                  __attribute__((used));
 off_t   _lseek  ( int fd, off_t offset, int whence )  __attribute__((used));
 ssize_t _read   ( int fd, void *x, size_t len )       __attribute__((used));
 void*   _sbrk   ( int incr )                          __attribute__((used));
 ssize_t _write  ( int fd, const void *x, size_t len ) __attribute__((used));

}


int _close( int fd )
{
	(void)fd;

	errno = ENOSYS;
	return -1;
}


void _exit( int status )
{
	(void)status;

	for( ;; )
		;
}


int _fstat( int fd, struct stat *st )
{
	(void)fd;
	(void)st;

	errno = ENOSYS;
	return -1;
}


int _getpid( void )
{
	errno = ENOSYS;
	return -1;
}


int _isatty( int fd )
{
	(void)fd;

	errno = ENOSYS;
	return 0;
}


int _kill( int pid, int sig )
{
	(void)pid;
	(void)sig;

	errno = ENOSYS;
	return -1;
}


off_t _lseek( int fd, off_t offset, int whence )
{
	(void)fd;
	(void)offset;
	(void)whence;

	errno = ENOSYS;
	return -1;
}


ssize_t _read( int fd, void *x, size_t len )
{
	(void)fd;
	(void)x;
	(void)len;

	errno = ENOSYS;
	return -1;
}


void* _sbrk( int incr )
{
	extern char end;
	static char *heap_end;
	char        *prev_end;

	if( heap_end == 0 )
		heap_end = &end;

	prev_end  = heap_end;
	heap_end += incr;

	return (void*)prev_end;
}


ssize_t _write( int fd, const void *x, size_t len )
{
	(void)fd;

	//(void)qb50::UART1.write( x, len );

	uint8_t *v = (uint8_t*)x;
	USART_TypeDef *U1 = (USART_TypeDef*)qb50::UART1.iobase;

	for( size_t i = 0 ; i < len ; ++i ) {
		while( !( U1->SR & USART_SR_TXE ));
		U1->DR = v[i];
	}

	// Always pretend that write() was successful, regardless of UART1
	// occasionnally dropping some of the data (i.e. _txFIFO overflow).

	return len;
}

/*EoF*/
