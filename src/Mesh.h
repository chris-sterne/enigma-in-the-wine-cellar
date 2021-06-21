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

#include <gtkmm.h>
#include <GL/gl.h>
#include <GL/glx.h>

class CMesh
{
  public:
    // Public classes.
    
    class CState
    {
      public:
        std::vector<GLfloat> iVertices;  // Vertices (x,y,z triples).
        std::vector<GLfloat> iColours;   // Vertex colours (r,g,b triples).
        std::vector<GLuint> iFaces;      // Face vertex array indices (triples).        
    };
    
    // Public data.
    
    CState iInactive;    // Inactive state mesh data.
    CState iActive;      // Active state mesh data.
};

#endif // __MESH_H__
