//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 5.0 for Linux OS          *
//* File:    MapPlayer.h               *
//* Date:    October 21, 2016          *
//* Author:  Chris Sterne              *
//*                                    *
//* MapPlayer class header.            *
//*------------------------------------*
 
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