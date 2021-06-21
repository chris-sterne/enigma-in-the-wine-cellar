//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 5.0 for Linux OS          *
//* File:    MapItem.h                 *
//* Date:    September 19, 2016        *
//* Author:  Chris Sterne              *
//*                                    *
//* MapItem class header.              *
//*------------------------------------*
 
#ifndef __MAPITEM_H__
#define __MAPITEM_H__

#include <gtkmm.h>
#include "EnigmaWC.h"
#include "MapObject.h"

class CMapItem : public CMapObject
{
  public:  
    // Public data.

    EnigmaWC::Category iCategory;  // Category of item.
    gboolean iActive;              // TRUE if item has yet to be found.
    gboolean iSelected;            // TRUE if item has been selected.
    gboolean iUsed;                // TRUE if item has been used.
    guint8 iOwner;                 // Player number owning item.
    
    // Saved values.
    
    gboolean iActiveSaved;         // Saved active.
    gboolean iSelectedSaved;       // Saved selected.
    gboolean iUsedSaved;           // Saved used.
    guint8 iOwnerSaved;            // Saved owner.

    // Restart values.

    gboolean iActiveRestart;       // Restart active.
    gboolean iSelectedRestart;     // Restart selected.
    gboolean iUsedRestart;         // Restart used.
    guint8 iOwnerRestart;          // Restart owner.
};

#endif // __MAPITEM_H__