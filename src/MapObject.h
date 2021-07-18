// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MapObject class header.  The MapObject class is the
// parent class of all objects in a game map.
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
 
#ifndef __MAPOBJECT_H__
#define __MAPOBJECT_H__

#include <gtkmm.h>
#include "EnigmaWC.h"
#include "MapLocation.h"
#include "Connection.h"

class CMapObject
{
  public:
    // Public data.

    EnigmaWC::ID iID;                 // Object ID.
    EnigmaWC::Direction iSurface;     // Object surface.
    EnigmaWC::Direction iRotation;    // Object rotation on surface.
    CMapLocation iLocation;           // Map location of object.
    CConnection iSense;               // Sense state.
    CConnection iState;               // Functional state.
    CConnection iVisibility;          // Visiblity state.
    CConnection iPresence;            // Presence state. 
};

#endif // __MAPOBJECT_H__
