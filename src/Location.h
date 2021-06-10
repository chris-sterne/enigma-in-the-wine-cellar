// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Location class header.  The Location class describes
// the location of an object within a game world.
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
 
#ifndef __ENIGMA_LOCATION_H__
#define __ENIGMA_LOCATION_H__

#include <glib.h>

namespace Enigma
{
	class Location
	{
		public:			
			// Public declarations.
			
			enum class Direction
			{
				NONE = 0,
				NORTH,
				SOUTH,
				EAST,
				WEST,
				ABOVE,
				BELOW,
				CENTER,
				TOTAL
			};
			
			// Public methods.

			Location();
			void clear();
			void set(guint16 east,
			         guint16 above,
			         guint16 north,
			         Enigma::Location::Direction surface,
			         Enigma::Location::Direction rotation);
			         
			static Enigma::Location::Direction
		         get_opposite(Enigma::Location::Direction direction);
		    
			static Enigma::Location::Direction
			       get_left(Enigma::Location::Direction surface,
			                Enigma::Location::Direction rotation);
			
			static Enigma::Location::Direction
			       get_right(Enigma::Location::Direction surface,
			                 Enigma::Location::Direction rotation);
			
			//Public data.
			
			guint16 m_east;                   // East position.
			guint16 m_north;                  // North position.
			guint16 m_above;                  // Above position.
			Location::Direction m_surface;    // Surface containing object.
			Location::Direction m_rotation;   // Rotation of object on surface.
	};
}

#endif // __ENIGMA_LOCATION_H__
