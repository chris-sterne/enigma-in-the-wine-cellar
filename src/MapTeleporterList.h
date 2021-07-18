// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MapTeleporterList class header.  The MapTeleporterList
// class manages a list of map teleporters, which are objects that can change
// a player's location.
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
 
#ifndef __MAPTELEPORTERLIST_H__
#define __MAPTELEPORTERLIST_H__

#include <gtkmm.h>
#include "MapTeleporter.h"

class CMapTeleporterList : public std::list<CMapTeleporter>
{
 public:
	  // Public methods.
		
    CMapTeleporterList();
    void Clear();

    void Read( const CMapLocation& aLocation,
               std::list<std::list<CMapTeleporter>::iterator>& aBuffer );
};

#endif // __MAPTELEPORTERLIST_H__
