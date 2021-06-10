// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Interaction class header.  The Interaction class is
// a data structure describing a player-object interaction.
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
 
#ifndef __ENIGMA_INTERACTION_H__
#define __ENIGMA_INTERACTION_H__

#include "Location.h"

namespace Enigma
{
	class Interaction
	{
		public:
			// Information from player.
		
			Enigma::Location m_location;
			Enigma::Location::Direction m_direction;
			                
			// Information from object.
			
			Enigma::Location::Direction m_next_direction;
			Enigma::Location::Direction m_next_rotation;
	
			short m_zone;                 // Interaction zone of object.
			bool m_blocks;                // TRUE if direction is blocked.
			bool m_supports;              // TRUE if object provides support.
	};
}

#endif // __ENIGMA_INTERACTION_H__
