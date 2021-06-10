// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Mesh class header  The Mesh class manages OpenGL image
// mesh data for a world object.
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
 
#ifndef __ENIGMA_MESH_H__
#define __ENIGMA_MESH_H__

#include <glibmm/object.h>
#include <vector>
#include <GL/gl.h>
#include "FineLocation.h"

namespace Enigma
{
	class Mesh
	{
		public:
			// Public methods.

			Mesh();
			void load(std::string name);
			void render(const Enigma::Location& object,
			            const Enigma::FineLocation& viewer);
			static void initialize();
			
		private:
			// Private data.

			GLuint m_position_bo;      // Position buffer object.
			GLuint m_colour_bo;		     // Colour buffer object.
	};
}
#endif // __ENIGMA_MESH_H__
