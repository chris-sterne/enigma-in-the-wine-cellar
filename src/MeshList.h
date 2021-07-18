// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MeshList class header.  The MeshList class renders image
// meshes with OpenGL.
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
 
#ifndef __MESHLIST_H__
#define __MESHLIST_H__

#include "MapPlayer.h"
#include <GL/gl.h>
#include "Mesh.h"

class CMeshList : public std::vector<CMesh>
{
  public:    
    // Public methods.

    CMeshList();

    void Render( const CMapObject& aObject,
                 const CMapPlayer& aViewer );
                 
    void Animate();   
    void Initialize();
                 
  private:
    // Private data.

    int iFishSwim;                // Fish swimming around viewer.
    int iFishTurn;                // Fish turning in place.
    int iFishState;               // Fish movement state.
    
		// Vertex shader attribute and uniform locations.  These will be initialized
		// after the shader program has been compiled and linked.

		GLint ivPosition;
		GLint ivColour;
		GLint imTransform;
};

#endif // __MESHLIST_H__
