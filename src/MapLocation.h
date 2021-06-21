//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 5.0 for Linux OS          *
//* File:    MapLocation.h             *
//* Date:    October 9, 2016           *
//* Author:  Chris Sterne              *
//*                                    *
//* MapLocation class header.          *
//*------------------------------------*
 
#ifndef __MAPLOCATION_H__
#define __MAPLOCATION_H__

#include <gtkmm.h>

class CMapLocation
{ 
  public:		
    // Public methods.
    
    CMapLocation();
    CMapLocation( guint16 aLevel, guint16 aRow, guint16 aColumn );
    void Clear();
    gboolean operator==( const CMapLocation& aLocation ) const;
    gboolean operator!=( const CMapLocation& aLocation ) const;
    
    // Public data.

		guint16 iEast;    // Room East location.
		guint16 iNorth;   // Room North location.
		guint16 iAbove;   // Room Above location.
};

#endif // __MAPLOCATION_H__