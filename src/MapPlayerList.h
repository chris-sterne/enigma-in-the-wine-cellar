// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MapPlayerList class header.  The MapPlayerList class
// manages a list of map players, which are objects that can move around
// and interact with other objects.
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
 
#ifndef __MAPPLAYERLIST_H__
#define __MAPPLAYERLIST_H__

#include <gtkmm.h>
#include "MapPlayer.h"

class CMapPlayerList : public std::list<CMapPlayer>
{
 public:
	  // Public methods.
		
    CMapPlayerList();
    void Clear();
    void Load();
    void Save();
    void Restart();
    std::list<CMapPlayer>::iterator GetActive();
    std::list<CMapPlayer>::iterator SwitchActive();
    
    gboolean Valid( std::list<CMapPlayer>::iterator& aPlayer );

    void Read( const CMapLocation& aLocation,
               std::list<std::list<CMapPlayer>::iterator>& aBuffer );
};

#endif // __MAPPLAYERLIST_H__
