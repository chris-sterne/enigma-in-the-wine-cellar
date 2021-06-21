//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 3.0 for Linux OS          *
//* File:    MapObject.h               *
//* Date:    April 8, 2016             *
//* Author:  Chris Sterne              *
//*                                    *
//* MapObject class header.            *
//*------------------------------------*
 
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