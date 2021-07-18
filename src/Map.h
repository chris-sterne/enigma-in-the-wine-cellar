// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Map class header.  The Map class manages a game map.
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
 
#ifndef __MAP_H__
#define __MAP_H__

#include <gtkmm.h>
#include "MapObjectList.h"
#include "MapTeleporterList.h"
#include "MapItemList.h"
#include "MapPlayerList.h"
#include "MapControllerList.h"

class CMap
{
  public:
    // Public methods.
		
    CMap();
    void Clear();
    void Restart();
    void Load();
    void Save();
    void LoadFile( const std::string& aFileName );
    void SaveFile(); 
    gboolean GetLoaded();
    gboolean GetSavable();    
    CMapControllerList& Controllers();
    CMapObjectList& Objects();
    CMapTeleporterList& Teleporters();
    CMapItemList& Items();
    CMapPlayerList& Players();
    std::string& Description();
    const CMapLocation& UpperBounds();
    const CMapLocation& LowerBounds();

    // Public data.
		
  private:
    // Private methods.
    
    void AdjustBoundary( const CMapLocation& aLocation );
  
    // Private data.

    std::string iFileName;            // Full file path name of game map.
    CMapControllerList iControllers;  // List of MapControllers.
    CMapObjectList iObjects;          // List of simple MapObjects.
    CMapTeleporterList iTeleporters;  // List of teleporter MapObjects.
    CMapItemList iItems;              // List of item MapObjects.
    CMapPlayerList iPlayers;          // List of player MapObjects.
    std::string iDescription;         // Description of game map in UTF-8 format.
    gboolean iSavable;                // TRUE if map can be saved internally.
    gboolean iBounded;                // TRUE if a map boundary has been set. 
    CMapLocation iLowerBounds;        // Lower map boundary.
    CMapLocation iUpperBounds;        // Upper map boundary.
};

#endif // __MAP_H__
