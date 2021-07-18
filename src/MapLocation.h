// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MapLocation class header.  The MapLocation class stores
// a location in a game map.
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
