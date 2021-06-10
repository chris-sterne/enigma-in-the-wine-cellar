// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Blocker class implementation.  A Blocker is an invisible
// wall that can block or support a player.
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
 
#include "Blocker.h"
#include "Mesh.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::Blocker::Blocker()
{
	// Customize base class data.
	
	m_id = Enigma::Object::ID::BLOCKER;
}

//---------------------------------------
// This method interacts with the object.
//---------------------------------------
// interaction: Interaction data.
//---------------------------------------

void Enigma::Blocker::interact(Enigma::Interaction& interaction)
{	
	// Get Blocker orientation.
	
	Enigma::Location::Direction surface = m_location.m_surface;
	
	Enigma::Location::Direction opposite_surface =
		Enigma::Location::get_opposite(surface);
	
	// Determine interaction zone based where the Blocker is located
	// along the interaction direction.
	
	if (interaction.m_direction == surface)
		interaction.m_zone = 3;
	else if (interaction.m_direction == opposite_surface)
		interaction.m_zone = -3;
	else
		interaction.m_zone = 0;

	// Determine the effects of a Blocker interaction with player.
	// Get player viewspace directions.

	Enigma::Location::Direction down = interaction.m_location.m_surface;

	// A Blocker under the player provides support, blocks interactions
	// perpendicular to its surface, and does not alter the interaction
	// direction, even if the interaction will be blocked.
	
	interaction.m_supports       = (surface == down);
	interaction.m_blocks         = (surface == interaction.m_direction);
	interaction.m_next_direction = interaction.m_direction;
}

//-------------------------------------------------------------------
// This static method initializes class resources.  An OpenGL context
// must be active so the object meshes can be loaded.
//-------------------------------------------------------------------

void Enigma::Blocker::initialize()
{
	// There are no class resources to initialize.
}
