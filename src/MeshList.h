//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 5.0 for Linux OS          *
//* File:    MeshList.h                *
//* Date:    October 14, 2016          *
//* Author:  Chris Sterne              *
//*                                    *
//* ImageMeshes class header.          *
//*------------------------------------*
 
#ifndef __MESHLIST_H__
#define __MESHLIST_H__

#include "MapPlayer.h"
#include "Mesh.h"

class CMeshList : public std::vector<CMesh>
{
  public:    
    // Public methods.

    CMeshList();

    void Render( const CMapObject& aObject,
                 const CMapPlayer& aViewer );
                 
    void Animate();
                 
  private:
    // Private data.

    int iFishSwim;                // Fish swimming around viewer.
    int iFishTurn;                // Fish turning in place.
    int iFishState;               // Fish movement state.
};

#endif // __MESHLIST_H__