//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 2.0 for Linux OS          *
//* File:    ObjectList.h              *
//* Date:    January 7, 2016           *
//* Author:  Chris Sterne              *
//*                                    *
//* ObjectList class header.           *
//*------------------------------------*
 
#ifndef __OBJECTLIST_H__
#define __OBJECTLIST_H__

#include <gtkmm.h>
#include "Object.h"

class CObjectList : public std::list<CObject>
{
  public:
		// Public methods.

		CObjectList();
		void Read( const CMapLocation& aLocation, std::list<CMapObject>& aBuffer );
		
		// Overloaded base class methods.

		clear();

	private:
		// Private methods.
		
		void Seek( const CMapLocation& aLocation );	

		// Private data.
		
		std::list<CObject>::iterator iIterator;   // Cached list iterator.
};

#endif // __OBJECTLIST_H__