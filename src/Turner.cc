// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Turner class implementation.  A Turner is an invisible
// object that turns a player.
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
 
#include "Turner.h"
#include "Mesh.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::Turner::Turner()
{
	// Customize base class data.
	
	m_id = Enigma::Object::ID::TURNER;
}

//---------------------------------------
// This method interacts with the object.
//---------------------------------------
// interaction: Interaction data.
//---------------------------------------

void Enigma::Turner::interact(Enigma::Interaction& interaction)
{
	// A Turner does not support or block a player, and does not alter
	// the interaction direction. 
	
	interaction.m_supports       = false;
	interaction.m_blocks         = false;
	interaction.m_next_direction = interaction.m_direction;
	
	// A central Turner or one on the same surface as the player will turn
	// the player.  The Turner's rotation is actually a number of quarter
	// turns to the right (0 = 0 degrees, 1 = 90 degrees, etc.), rather
	// than an object rotation.
	
	if  ((m_location.m_surface == Enigma::Location::Direction::CENTER)
	  || (m_location.m_surface == interaction.m_location.m_surface))
	{
		interaction.m_next_rotation = interaction.m_location.m_rotation;
		short quarters = (short)m_location.m_rotation;
		
		while (quarters > 0)
		{
			// Change the next rotation direction by 90 degrees. 
			
			interaction.m_next_rotation =
				Enigma::Location::get_right(interaction.m_location.m_surface,
				                            interaction.m_next_rotation);

			quarters --;
		}
	}
}

//-------------------------------------------------------------------
// This static method initializes class resources.  An OpenGL context
// must be active so the object meshes can be loaded.
//-------------------------------------------------------------------

void Enigma::Turner::initialize()
{
	// There are no class resources to initialize.
}
