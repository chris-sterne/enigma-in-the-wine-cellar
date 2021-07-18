// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MapPlayer class header.  The MapPlayer class describes
// a player in a game map.
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
 
#ifndef __MAPPLAYER_H__
#define __MAPPLAYER_H__

#include <gtkmm.h>
#include "EnigmaWC.h"
#include "MapObject.h"
#include "MapItem.h"
#include "FinePoint.h"

class CMapPlayer : public CMapObject
{
  public:    
    // Public data.

    guint iNumber;                            // Player number.
    gboolean iActive;                         // TRUE if active.
    gboolean iOutdoor;                        // TRUE if outdoor.
    gboolean iWaterLayer;                     // TRUE if in WaterLayer.
    gboolean iWater;                          // TRUE if under Water.
    gboolean iInnerTube;                      // TRUE if using InnerTube.
    gboolean iHydrogenBalloon;                // TRUE if using HydrogenBalloon.
    EnigmaWC::Direction iStairsSurface;       // Stairs surface.
    EnigmaWC::Direction iStairsRotation;      // Stairs rotation.
    CFinePoint iOffset;                       // Fine orientation offset.
           
    // Saved values.
    
    EnigmaWC::Direction iSurfaceSaved;        // Saved surface.
    EnigmaWC::Direction iRotationSaved;       // Saved rotation.
    CMapLocation iLocationSaved;              // Saved location.
    gboolean iActiveSaved;                    // Saved active.
    gboolean iOutdoorSaved;                   // Saved outdoor.
    
    // Restart values.
        
    EnigmaWC::Direction iSurfaceRestart;      // Restart surface.
    EnigmaWC::Direction iRotationRestart;     // Restart rotation.
    CMapLocation iLocationRestart;            // Restart location.
    gboolean iActiveRestart;                  // Restart active.
    gboolean iOutdoorRestart;                 // Restart outdoor.		

    // Destination values.
    
    EnigmaWC::Direction iSurfaceNext;         // Destination surface.
    EnigmaWC::Direction iRotationNext;        // Destination rotation.
    CMapLocation iLocationNext;               // Destination location.
    EnigmaWC::Direction iStairsSurfaceNext;   // Destination Stairs surface.
    EnigmaWC::Direction iStairsRotationNext;  // Destination Stairs rotation.
    gboolean iOutdoorNext;                    // Destination outdoor state.
    
    // PlayRoom contact bits.

    guint8 iContactBits;                      // Cause PlayRoom contact.
    guint8 iAntiContactBits;                  // Prevent PlayRoom contact.
};

#endif // __MAPPLAYER_H__
