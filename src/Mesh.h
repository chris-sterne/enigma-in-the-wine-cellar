// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Mesh class header.  The Mesh class describes a map object
// OpenGL image.
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
 
#ifndef __MESH_H__
#define __MESH_H__

#include <GL/gl.h>

class CMesh
{
	public:
		CMesh()
		{
			iInitialized = false;
		}
	
	// Public classes.

	class CState
	{
		public:
			CState()
			{
				// Set Buffer Object "names" to uninitialized value.
				
				iPositionBO = 0;
				iColourBO   = 0;
			}
		
			std::vector<GLfloat> iVertices;   // Vertices (x,y,z triples).
			std::vector<GLfloat> iColours;    // Vertex colours (r,g,b triples).
			GLuint iPositionBO;               // Position buffer object.
			GLuint iColourBO;                 // Colour buffer object.    
	};

	// Public data.

	CState iInactive;            // Inactive state mesh data.
	CState iActive;              // Active state mesh data.
	bool iInitialized;           // TRUE if initialized.
};

#endif // __MESH_H__
