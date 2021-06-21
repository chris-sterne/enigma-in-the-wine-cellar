//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 5.0 for Linux OS          *
//* File:    MapItemList.h             *
//* Date:    October 14, 2016          *
//* Author:  Chris Sterne              *
//*                                    *
//* MapItemList class header.          *
//*------------------------------------*
 
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