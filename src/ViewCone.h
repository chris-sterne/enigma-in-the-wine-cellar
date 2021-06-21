//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 5.0 for Linux OS          *
//* File:    ViewCone.h                *
//* Date:    October 13, 2016          *
//* Author:  Chris Sterne              *
//*                                    *
//* ViewCone class header.             *
//*------------------------------------*

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