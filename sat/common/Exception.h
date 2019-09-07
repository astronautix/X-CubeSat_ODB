#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED


#include <FreeRTOS.h>
#include <exception>
#include <iostream>
#include <string>

using namespace std;

namespace qb50 {

    class Exception : public exception
    {
        public:

            Exception( const char* erreur) throw() : _erreur(erreur)
            {
                ;
            }
            virtual ~Exception() throw()
            {
                ;
            }

            virtual const char* what() throw()
            {
                return _erreur;
            }


        private:

            const char* _erreur; // erreur à renvoyer
    };

}



#endif /* EXCEPTION_H_INCLUDED */
