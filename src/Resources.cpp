//*----------------------------------------------*
//* Program: Enigma in the Wine Cellar           *
//* Version: 3.0 for Linux OS                    *
//* File:    Resources.cpp                       *
//* Date:    June 13, 2016                       *
//* Author:  Chris Sterne                        *
//*----------------------------------------------*
//* This class provides access to resource data. *
//* ---------------------------------------------*

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

//*-----------------------------------------------------------------------*
//* This private function reads image mesh state data from the resources. *
//*-----------------------------------------------------------------------*

void ReadStateData( CMesh::CState& aState, const std::string& aPath )
{
  // Find the data in the resources.  Due to the Giomm Gio::Resource
  // declaration being unrecognized, the C version of the lookup
  // function is used instead.
	
  GResource* Resource	= EnigmaWC_get_resource();
  GError* Error				= NULL;
	
  GBytes* Data = g_resource_lookup_data( Resource,
                                         aPath.data(),
                                         G_RESOURCE_LOOKUP_FLAGS_NONE,
                                         &Error);

  // Return immediately if the resource could not be found.
	
  if ( Data == NULL )
    return;

  gsize ArrayLength;
  guchar* Array = (guchar*)g_bytes_get_data( Data, &ArrayLength );

  // Return immediately if the resource data could not obtained.

  if (( Array == NULL ) || ( ArrayLength == 0 ))
    return;

	// Fill the image mesh with data from its resource.
	
	std::vector<Glib::ustring> Arguments;
	std::vector<Glib::ustring> Elements;
	std::vector<int> Totals;

	std::string Line;
	guint ArgumentCount;
		
	// Read information from the mesh file header.
	
	while ( ArrayLength > 0 )
	{
		ReadLine( Line, &Array, &ArrayLength );
		
		Arguments     = Glib::Regex::split_simple( " |\n|\r", Line );
		ArgumentCount = Arguments.size();

		if ( ArgumentCount > 0 )
		{
			if ( Arguments.at( 0 ).compare( "element" ) == 0 )
			{
				if ( Arguments.at( 1 ).compare( "vertex" ) == 0 )
				{
					Elements.push_back( Arguments.at( 1 ) );
					Totals.push_back( std::stoi( Arguments.at( 2 ) ));
				}
				else if ( Arguments.at( 1 ).compare( "face" ) == 0 )
				{
					Elements.push_back( Arguments.at( 1 ) );
					Totals.push_back( std::stoi( Arguments.at( 2 ) ));
				}
			}
			else if ( Arguments.at( 0 ).compare( "end_header" ) == 0 )
			{
				break;
			}
		}
	}

	// The very next line following the header will be the first line
	// of the first block of element data.

	guint Block = 0;
	guint Counter;
		
	while ( Block < Totals.size() )
	{
		Counter = Totals.at( Block );
		
		if ( Elements.at( Block ).compare( "vertex" ) == 0 )
		{
			// Reserve vector space for the number of vertex values
			// to be added.  This prevents possible multiple memory
			// re-allocations as values are added to the vector.
			
			aState.iVertices.reserve( Counter * 6 );
			
			// Read all vertex data.
						
      union
      {
        GLuint Integer;
        GLfloat Float;
      } Convert;
      
      guint SubCounter; 
     
      while ( Counter != 0 ) 
      {        
        // Extract vertex coordinates (x, y, z) stored as 4-byte floating-point
        // values in big-endian format.  The coordinate components are
        // converted into floating values for OpenGL.
			 
			  SubCounter = 3;
			 
        do
        {
          Convert.Integer = (GLuint)(( *(Array + 0) << 24 )
                                   | ( *(Array + 1) << 16 )
                                   | ( *(Array + 2) << 8 )
                                   |   *(Array + 3) );
				  Array += 4;
				  
          aState.iVertices.push_back( Convert.Float );
          -- SubCounter;
        }
        while ( SubCounter > 0 );
        								
        // Extract vertex colours (r, g, b) stored as 1-byte values  These
        // colour components must be converted into floating values for OpenGL.
			 
        SubCounter = 3;
			
        do
        {	        
          aState.iColours.push_back( (GLfloat)(*Array) / 255 );
          ++ Array;
          -- SubCounter;
        }
        while ( SubCounter > 0 );

				// Decrement the vertex counter.

				-- Counter;
			}
		}
		else if ( Elements.at( Block ).compare( "face" ) == 0 )
		{
			// Reserve vector space for the number of face values to be added.
			// This prevents possible multiple memory re-allocations as values
			// are added to the vector.

			aState.iFaces.reserve( Counter * 3 );

			// Read all face data.
			
			guint SubCounter;
			GLuint Integer;

			while ( Counter != 0 ) 
			{
			  // Extract the vertex indices counter stored as a 1-byte value.
			
			  SubCounter = (guint)(*Array);
			  ++ Array;
				
				// Extract vertex indices stored as 2-byte values in big-endian
				// format.  The indices are converted into integer values for OpenGL.
			
        do
        {
          Integer = (GLuint)(( *(Array + 0) << 8 )
                             | *(Array + 1) );
				  Array += 2;
				  
          aState.iFaces.push_back( Integer );
          -- SubCounter;
        }
        while ( SubCounter > 0 );

				// Decrement the face counter.

				-- Counter;
			}
		}

		// Move to the next block of elements.
		
		Block ++;
	}
	
	g_bytes_unref( Data );
  return;
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
  FullName.append( "_N.ply");
  ReadStateData( aMesh.iInactive, FullName );
 
  // Read active mesh data.
 
  FullName = PathName;
  FullName.append( "_A.ply");
  ReadStateData( aMesh.iActive, FullName );
  
  return;
}
