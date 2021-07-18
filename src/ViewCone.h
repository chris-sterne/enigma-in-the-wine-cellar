// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the ViewCone class header.  The ViewCone class handles drawing
// of the player's viewing cone.
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

#ifndef VIEWCONE_H_
#define VIEWCONE_H_

#include "EnigmaWC.h"
#include "MeshList.h"
#include "Map.h"

class CViewCone : public sigc::trackable
{
  public:        
    // Public methods.

    CViewCone();
    void SetDepth( gint aDepth );
    void SetMap( std::shared_ptr<CMap> aMap );
    gint64 GetRenderTime();
    void Fill( guint8 aSelect );
    void Render();
    void Initialize();

  private:
    // Private data.

    std::shared_ptr<CMap> iMap;                // Game map being viewed.
    std::list<CMapPlayer>::iterator iPlayer;   // Active player.
    CMeshList iMeshList;                       // Image mesh drawing object.
    gint iDepth;                               // Depth of viewing cone.
    gint64 iRenderTime;                        // Rendering time.
    Glib::Rand iRandom;                        // Random number generator.
   
    // View objects and lists.
    
    CMapObject iSkyObjects;
    CMapObject iUseObject;
    std::list<std::list<CMapObject>::iterator> iEnvironments;
    std::list<std::list<CMapObject>::iterator> iObjects;
    std::list<std::list<CMapTeleporter>::iterator> iTeleporters;
    std::list<std::list<CMapItem>::iterator> iItems;
    std::list<std::list<CMapPlayer>::iterator> iPlayers;
};

#endif /* VIEWCONE_H_ */
