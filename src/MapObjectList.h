//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 2.0 for Linux OS          *
//* File:    MapObjectList.h           *
//* Date:    January 20, 2016          *
//* Author:  Chris Sterne              *
//*                                    *
//* MapObjectList class header.        *
//*------------------------------------*
 
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