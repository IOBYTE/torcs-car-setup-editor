/*  Torcs Car Setup Editor. A visual car setup editor for torcs build using glui
    Copyright (C) 2007  Vicente Marti

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include "portability.h"

#ifdef WIN32
#include <dir.h>

bool makedir( const char* dir )
{
	mkdir( dir );

	return true; //Return error instead if failed?
}
#else
#include <sys/stat.h>

bool makedir( const char* dir )
{
	mkdir( dir, 0755 );
}
#endif //WIN32
