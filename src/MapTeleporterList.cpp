//*---------------------------------------------------------*
//* Program: Enigma in the Wine Cellar                      *
//* Version: 5.0 for Linux OS                               *
//* File:    MapTeleporterList.cpp                          *
//* Date:    September 15, 2016                             *
//* Author:  Chris Sterne                                   *
//*---------------------------------------------------------*
//* This class manages a list of map teleporters, which are *
//* objects that can change the player's orientation.       *
//*---------------------------------------------------------*

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
