/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:19:32+02:00
 * @License: This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * @Copyright: Copyright (C) 2027 Olivier Piras  
 */
 
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

            const char* _erreur; // erreur � renvoyer
    };

}



#endif /* EXCEPTION_H_INCLUDED */
