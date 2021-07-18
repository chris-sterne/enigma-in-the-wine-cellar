// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MapItem class header.  The MapItem class describes
// an item that can be found and used by a player. 
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
