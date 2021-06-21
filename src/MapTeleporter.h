//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 5.0 for Linux OS          *
//* File:    MapTeleporter.h           *
//* Date:    September 15, 2016        *
//* Author:  Chris Sterne              *
//*                                    *
//* MapTeleporter class header.        *
//*------------------------------------*
 
#ifndef __MAPTELEPORTER_H__
#define __MAPTELEPORTER_H__

#include <gtkmm.h>
#include "EnigmaWC.h"
#include "MapObject.h"

class CMapTeleporter : public CMapObject
{
  public:        
    // Public data.
    
    EnigmaWC::Direction iSurfaceArrival;   // Arrival surface.
    EnigmaWC::Direction iRotationArrival;  // Arrival rotation on surface.
    CMapLocation iLocationArrival;         // Arrival location.
};

#endif // __MAPTELEPORTER_H__