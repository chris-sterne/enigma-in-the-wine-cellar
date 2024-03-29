// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the ObjectList class header.  The ObjectList class stores
// map objects as a pre-sorted manner.
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <http://www.gnu.org/licenses/>.
 
#ifndef __OBJECTLIST_H__
#define __OBJECTLIST_H__

#include <gtkmm.h>
#include "Object.h"

class CObjectList : public std::list<CObject>
{
  public:
		// Public methods.

		CObjectList();
		void Read( const CMapLocation& aLocation, std::list<CMapObject>& aBuffer );
		
		// Overloaded base class methods.

		clear();

	private:
		// Private methods.
		
		void Seek( const CMapLocation& aLocation );	

		// Private data.
		
		std::list<CObject>::iterator iIterator;   // Cached list iterator.
};

#endif // __OBJECTLIST_H__
