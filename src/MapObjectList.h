// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MapObjectList class header.  The MapObjectList class
// manages a list of map objects.  The list contents are sorted according
// to their map location, and the list iterator is cached for faster locating
// of objects.
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
 
#ifndef __MAPOBJECTLIST_H__
#define __MAPOBJECTLIST_H__

#include <gtkmm.h>
#include "MapObject.h"

class CMapObjectList : public std::list<CMapObject>
{
 public:
	  // Public methods.
		
    CMapObjectList();
		void Clear();
		void Seek( const CMapLocation& aLocation );

		void Read( const CMapLocation& aLocation,
               std::list<std::list<CMapObject>::iterator>& aBuffer );
	private:
	  // Private data.

		std::list<CMapObject>::iterator iIterator;  // Cached list iterator.
};

#endif // __MAPOBJECTLIST_H__
