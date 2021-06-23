//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 2.0 for Linux OS          *
//* File:    Mesh.h                    *
//* Date:    June 13, 2016             *
//* Author:  Chris Sterne              *
//*                                    *
//* Mesh class header.                 *
//*------------------------------------*
 
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
