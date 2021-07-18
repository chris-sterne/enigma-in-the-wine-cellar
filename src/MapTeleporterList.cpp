// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MapTeleporterList class implementation.
// The MapTeleporterList class manages a list of map teleporters, which are
// objects that can change a player's location.
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

#include "MapTeleporterList.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CMapTeleporterList::CMapTeleporterList()
{	
	return;
}

//*----------------------------------------------------------------*
//* This method return the teleporter list to the empty condition. *
//*----------------------------------------------------------------*

void CMapTeleporterList::Clear()
{
	// Clear the list in the base class.
	
	clear();
	return;
}

//*---------------------------------------------------------------------*
//* This method returns iterators to all teleporters in a map location. *
//*---------------------------------------------------------------------*
//* aLocation: Map location to be examined.                             *
//* aBuffer:   Buffer to receive copies of MapTeleporter iterators.     *
//*---------------------------------------------------------------------*

void CMapTeleporterList::Read( const CMapLocation& aLocation,
                    std::list<std::list<CMapTeleporter>::iterator>& aBuffer )
{
  std::list<CMapTeleporter>::iterator Teleporter;

  for ( Teleporter = begin();
        Teleporter != end();
        ++ Teleporter )
  {
    // If the teleporter location is correct, add a copy of the teleporter
    // list iterator to the provided buffer. 
    
    if ( (*Teleporter).iLocation == aLocation )
      aBuffer.push_back( Teleporter );
  }
  
  return;
}
