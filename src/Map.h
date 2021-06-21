//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 5.0 for Linux OS          *
//* File:    Map.h                     *
//* Date:    October 12, 2016          *
//* Author:  Chris Sterne              *
//*                                    *
//* Map class header.                  *
//*------------------------------------*
 
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