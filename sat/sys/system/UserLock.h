
#ifndef _QB50_SYS_USERLOCK_H
#define _QB50_SYS_USERLOCK_H

#include <FreeRTOS.h>
#include <semphr.h>

#include "system/Lock.h"


namespace qb50 {

class UserLock : public Lock
{

	public:

		UserLock();
		~UserLock();

		void P( void );
		void V( void );

	private:

		xSemaphoreHandle _lock;

};

} /*qb50*/


#endif /*_QB50_SYS_USERLOCK_H*/

/*EoF*/
