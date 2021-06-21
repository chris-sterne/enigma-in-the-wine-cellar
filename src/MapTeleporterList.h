//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 5.0 for Linux OS          *
//* File:    MapTeleporterList.h       *
//* Date:    September 15, 2016        *
//* Author:  Chris Sterne              *
//*                                    *
//* MapTeleporterList class header.    *
//*------------------------------------*
 
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