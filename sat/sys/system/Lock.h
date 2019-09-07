
#ifndef _QB50_SYS_LOCK_H
#define _QB50_SYS_LOCK_H


namespace qb50 {

class Lock
{

	public:

		virtual ~Lock() { ; }

		// see https://en.wikipedia.org/wiki/Semaphore_(programming)#Operation_names

		virtual void P( void ) = 0;
		virtual void V( void ) = 0;

		void acquire ( void ) __attribute__((always_inline))
		{ P(); }

		void release ( void ) __attribute__((always_inline))
		{ V(); }

};

} /* qb50 */


#endif /*_QB50_SYS_LOCK_H*/

/*EoF*/
