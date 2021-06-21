//*------------------------------------------------------*
//* Program: Enigma in the Wine Cellar                   *
//* Version: 2.0 for Linux OS                            *
//* File:    MapObjectList.cpp                           *
//* Date:    April 8, 2016                               *
//* Author:  Chris Sterne                                *
//*------------------------------------------------------*
//* This class manages a list of map objects.  The list  *
//* contents are sorted according to their map location, *
//* and the list iterator is cached for faster locating  *
//* of objects.                                          *       
//*------------------------------------------------------*

#include "MapObjectList.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CMapObjectList::CMapObjectList()
{	
	// Initialize the cached Object iterator.
	
	iIterator = end();
	return;
}

//*---------------------------------------------------------*
//* This method return the game map to the empty condition. *
//*---------------------------------------------------------*

void CMapObjectList::Clear()
{
	// Clear the list in the base class.
	
	clear();
	
	// Initialize the cached Object iterator.

	iIterator = end();
	return;
}

//*-------------------------------------------------------------------*
//* This method moves the list iterator just past an object whose     *
//* location (East, North, Above) matches the one provided.           *
//* If none exist, the iterator will point past the end of the list.  *
//* All objects are sorted according to their location.               *
//*-------------------------------------------------------------------*
//* aLocation: Map location to seek.                                  *
//*-------------------------------------------------------------------*

void CMapObjectList::Seek( const CMapLocation& aLocation )
{
	// Move the list iterator if the East, North, and Above are too high. 
	
	while (( iIterator != begin() )
	  && 
			(( iIterator == end() )
		||
		  ( (*iIterator).iLocation.iAbove > aLocation.iAbove )
		||
			(( (*iIterator).iLocation.iAbove == aLocation.iAbove )
		&& ( (*iIterator).iLocation.iNorth > aLocation.iNorth ))
		|| 
			(( (*iIterator).iLocation.iAbove == aLocation.iAbove )
		&& ( (*iIterator).iLocation.iNorth == aLocation.iNorth )
		&& ( (*iIterator).iLocation.iEast > aLocation.iEast ))))
	{
		iIterator --;
	}

	// Move the list iterator if East, North, and Above are too low
	// or equal.
	
	while (( iIterator != end() )
		&& (( (*iIterator).iLocation.iAbove < aLocation.iAbove )
		||
			(( (*iIterator).iLocation.iAbove == aLocation.iAbove )
		&& ( (*iIterator).iLocation.iNorth < aLocation.iNorth ))
		|| 
			(( (*iIterator).iLocation.iAbove == aLocation.iAbove )
		&& ( (*iIterator).iLocation.iNorth == aLocation.iNorth )
		&& ( (*iIterator).iLocation.iEast <= aLocation.iEast ))))
	{
		iIterator ++;
	}
	
	return;
}

//*-----------------------------------------------------------------*
//* This method returns iterators to all objects in a map location. *
//*-----------------------------------------------------------------*
//* aLocation: Map location to be examined.                         *
//* aBuffer:   Buffer to receive copies of MapObject iterators.     *
//*-----------------------------------------------------------------*

void CMapObjectList::Read( const CMapLocation& aLocation,
                           std::list<std::list<CMapObject>::iterator>& aBuffer )
{
	Seek( aLocation );

	while ( iIterator != begin() )
	{
		// Since the list iterator following a seek will be on the object
		// just past a desired object (or past the end of the list),
		// move the iterator back.

		iIterator --;

    if ( (*iIterator).iLocation == aLocation )
		{
			// An object is present with the correct location.  Copy the iterator
      // to this object into the provided buffer.

			aBuffer.push_back( iIterator );
		}
		else
			break;
	}

	return;
}
