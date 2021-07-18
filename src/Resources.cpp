// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Resource class implementation.  The Resource class
// provides access to resource data.
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

#include "EnigmaWC.gresource.h"
#include "Resources.h"

//*------------------------------------------------------------------------*
//* This method is called by Cairo::ImageSurface::create_from_png_stream() *
//* in the CreateImage() method.  PNG image resource data is returned.     *
//*------------------------------------------------------------------------*
//* aData:   Destination data buffer pointer.                              *
//* aLength: Amount of data to write.                                      *
//* RETURN:  Status of operation (always CAIRO_STATUS_SUCCESS)             *
//*------------------------------------------------------------------------*

cairo_status_t CResources::read_data( guchar *aData, guint aLength )
{
  memcpy( aData, iData, aLength );
  iData += aLength;
 
	return CAIRO_STATUS_SUCCESS;
}

//*----------------------------------------------------------*
//* This method creates an image surface from resource data. *
//*----------------------------------------------------------*
//* aName:  Full path name of resource file.                 *
//* RETURN: Image surface, or NULL if unsuccessful.          *
//*----------------------------------------------------------*

Cairo::RefPtr<Cairo::ImageSurface> CResources::CreateImageSurface( const std::string& aName )
{
	// Create a RefPtr to hold the created image surface.
	// This new RefPtr will be NULL (empty) by default.
	
	Cairo::RefPtr<Cairo::ImageSurface> Surface;

	// Find the data in the resource.  Due to the Giomm Gio::Resource
	// declaration being unrecognized, the C version of the lookup
	// function is used instead.

	GResource* Resource	= EnigmaWC_get_resource();
	GError* Error				= NULL;
	
  GBytes* Data = g_resource_lookup_data( Resource,
                                         aName.data(),
                                         G_RESOURCE_LOOKUP_FLAGS_NONE,
                                         &Error);

	// Return immediately with an empty image surface if the resource
	// could not be found.
	
	if ( Data == NULL )
		return Surface;

	gsize DataSize;
	iData = (guint8 *)g_bytes_get_data( Data, &DataSize );
	
	// Return immediately with an empty image mesh if the resource
	// data could not obtained.
	
	if (( iData == NULL ) || ( DataSize == 0 ))
		return Surface;

	// Attach a signal slot containing the resource data read method,
	// then read in the PNG image data.

	Surface = 
		Cairo::ImageSurface::create_from_png_stream
			( sigc::mem_fun( this, &CResources::read_data ));

	g_bytes_unref( Data );
	
	return Surface;
};

//*----------------------------------------------------------------------------*
//* This private function fills a buffer with one line from a character array. *
//* The line can be terminated with either a NewLine or Carriage Return.       *
//*----------------------------------------------------------------------------*
//* aString: String buffer to be filled.                                       *
//* aArray:  Array of characters.                                              *
//* aLength: Length of character array.                                        *
//*----------------------------------------------------------------------------*

void ReadLine( std::string& aString, guchar** aArray, gsize* aLength )
{
	aString.clear();
	
	// Read characters into string buffer until a NewLine or Carriage Return
	// characters is found.

	guchar* Array = *aArray;
	gsize Length  = *aLength;
	
	while (( *Array != '\n' )
		&&  ( *Array != '\r' )
	  &&  ( Length > 0 ))
	{
		aString.push_back( *Array );

		// Move to the next character in the array.
		
		++ Array;
		-- Length;
	}

	// Add a NULL-terminator to the string line.

	aString.push_back( '\x00' );
	
	// Skip over the line terminator character.
	
	if ( Length > 0 )
	{
		++ Array;
		-- Length;
	}

	// Update the array pointer and length of array remaining. 

	*aArray  = Array;
	*aLength = Length;
	
	return;
}

//----------------------------------------------------------------------
// This private function reads image mesh state data from the resources.
//----------------------------------------------------------------------
// aPath: Full path to mesh resource.
//----------------------------------------------------------------------

void ReadStateData( CMesh::CState& aState, const std::string& aPath )
{  
  // Extract the mesh data from the resource bundle.  Multi-byte values
	// in this bundle are in little-endian format.
	
	Glib::RefPtr<const Glib::Bytes> byte_array;
	
	try
  {	
		byte_array =
			Gio::Resource::lookup_data_global(aPath,
			                                  Gio::RESOURCE_LOOKUP_FLAGS_NONE);
	}
	catch( Glib::Error error )
	{
		return;
	}

	gsize size;
	guint8* byte = (guint8*)byte_array->get_data(size);
	
	// Return immediately if there is too little remaining data,
	// or the header ID ("mesh\n") is wrong.

	if (size < 4)
		return;
	
	if  ((*(byte + 0) != 'm')
		|| (*(byte + 1) != 's')
		|| (*(byte + 2) != 'h')
		|| (*(byte + 3) != '\n'))
	{	
		return;
	}
	
	// Skip over header ID to the vertex total (4 bytes).
	// Return immediately if there is too little remaining data.
	
	byte += 4;
	size -= 4;
	
	if (size < 4)
		return;
	
	int vertex_total = (*(byte + 0) 
	                 + (*(byte + 1) << 8)
	                 + (*(byte + 2) << 16)
	                 + (*(byte + 3) << 24));
	
	// Skip over the vertex total to the vertex data (array of 4-byte values).
	// Return immediately if there is too little remaining data.  Each vertex
	// coordinate uses 12 bytes (x,y,z 3-tuples), and each vertex colour uses
	// 16 bytes (r,g,b,a 4-tuples).

	byte += 4;
	size -= 4;
		
	if (size < vertex_total * (12 + 16))
		return;
	
	// Declare a structure to convert from GLuint and GLfloat types.
	
	union
	{
		GLuint gl_integer;
		GLfloat gl_float;
	} convert;

	// Stream resource data into the vertex position buffer.
	
	aState.iVertices.reserve(vertex_total * 3);
	int total = vertex_total * 3;
	
	while (total > 0)
	{
		// Extract a vertex position stored as a 4-byte OpenGL integer
		// value, and convert it into an OpenGL floating-point value.
		
		convert.gl_integer = (*(byte + 0) 
                       + (*(byte + 1) << 8)
                       + (*(byte + 2) << 16)
                       + (*(byte + 3) << 24));
		
		aState.iVertices.push_back(convert.gl_float);
		//position.emplace_back(convert.gl_float);
		
		total -= 1;
		byte  += 4;
	}
	
	// Stream resource data into the vertex colour buffers.
	
	aState.iColours.reserve(vertex_total * 4);
	total = vertex_total * 4;
	
	while (total > 0)
	{
		// Extract a vertex colour stored as a 4-byte OpenGL integer
		// value, and convert it to an OpenGL floating-point value.
		
		convert.gl_integer = (*(byte + 0) 
                       + (*(byte + 1) << 8)
                       + (*(byte + 2) << 16)
                       + (*(byte + 3) << 24));
                      
		aState.iColours.push_back(convert.gl_float);
		//colour.emplace_back(convert.gl_float);
		
		total -= 1;
		byte  += 4;
	}
}

//*------------------------------------------------------------*
//* This method loads an image mesh from resource data.        *
//*------------------------------------------------------------*
//* aMesh: Reference to image mesh to be filled.               *
//* aName: Root name of mesh resource file (e.g. "BlockWall"). *
//*------------------------------------------------------------*

void CResources::LoadImageMesh( CMesh& aMesh, const std::string& aName )
{
  std::string PathName = "/org/game/EnigmaWC/Meshes/";
  PathName.append( aName );
  
  // Read inactive mesh data.
  
  std::string FullName = PathName;
  FullName.append( "_N.msh");
  ReadStateData( aMesh.iInactive, FullName );
 
  // Read active mesh data.
 
  FullName = PathName;
  FullName.append( "_A.msh");
  ReadStateData( aMesh.iActive, FullName );
  
  return;
}
