//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 1.0 for Linux OS          *
//* File:    Resources.h               *
//* Date:    June 13, 2016             *
//* Author:  Chris Sterne              *
//*                                    *
//* Resources class header.            *
//*------------------------------------*

#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <gtkmm.h>
#include "Mesh.h"

class CResources
{
	public:
    // Public methods.

		Cairo::RefPtr<Cairo::ImageSurface> CreateImageSurface( const std::string& aPath );
		void LoadImageMesh( CMesh& aMesh, const std::string& aPath );

	private:
		// Private methods.
		
		cairo_status_t read_data( guchar *aData, guint aLength );

		// Private data.

		guint8* iData;     // Pointer to resource data.
};

#endif /* RESOURCES_H_ */