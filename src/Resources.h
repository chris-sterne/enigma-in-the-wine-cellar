// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Resource class header.  The Resource class provides access
// to resource data.
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
