// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Boundary class header.  The Boundary class describes
// the position limits of a game world.
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
 
#ifndef __ENIGMA_BOUNDARY_H__
#define __ENIGMA_BOUNDARY_H__

#include "Interaction.h"

namespace Enigma
{
	class Boundary
	{
		public:
			// Public methods.
			
			Boundary();
			void clear();
			bool is_clear();
			void interact(Enigma::Interaction& interaction);
			void adjust(const Enigma::Location& location);
			void adjust(guint16 east,
			            guint16 above,
                  guint16 north);

			// Lower corner.
			
			guint16 m_lower_east;
			guint16 m_lower_above;
			guint16 m_lower_north;
			
			// Upper corner.
			
			guint16 m_upper_east;
			guint16 m_upper_above;
			guint16 m_upper_north;
	};
}

#endif // __ENIGMA_BOUNDARY_H__
