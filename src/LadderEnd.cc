// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the LadderEnd class implementation.  A LadderEnd object is
// a short section of a Ladder that the player can use to climb up or down.
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
 
#include "LadderEnd.h"
#include "Mesh.h"

//--------------------
// Local declarations.
//--------------------

static const std::string active_name = "LadderEnd_A.msh";
static Enigma::Mesh active_mesh;

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::LadderEnd::LadderEnd()
{
	// Customize base class data.
	
	m_id = Enigma::Object::ID::LADDEREND;
}

//----------------------------------------------------------------------------
// This method is called within an active OpenGL context to render the object.
//----------------------------------------------------------------------------
// viewer: Location of viewer in the world.
//----------------------------------------------------------------------------

void Enigma::LadderEnd::render(const Enigma::FineLocation& viewer)
{
	active_mesh.render(m_location, viewer);
}

//---------------------------------------
// This method interacts with the object.
//---------------------------------------
// interaction: Interaction data.
//---------------------------------------

void Enigma::LadderEnd::interact(Enigma::Interaction& interaction)
{
	// Get LadderEnd orientation.
	
	Enigma::Location::Direction surface  = m_location.m_surface;
	Enigma::Location::Direction rotation = m_location.m_rotation;
	
	Enigma::Location::Direction opposite_surface =
		Enigma::Location::get_opposite(surface);
		
	Enigma::Location::Direction opposite_rotation =
		Enigma::Location::get_opposite(rotation);
				
	// Determine interaction zone based where the LadderEnd is located
	// along the interaction direction.

	if (interaction.m_direction == surface)
		interaction.m_zone = 2;
	else if (interaction.m_direction == rotation)
		interaction.m_zone = 1;
	else if (interaction.m_direction == opposite_rotation)
		interaction.m_zone = -1;	
	else if (interaction.m_direction == opposite_surface)
		interaction.m_zone = -2;
	else
		interaction.m_zone = 0;
		
	// Determine the effects of a LadderEnd interaction with player. In general,
	// a LadderEnd provides support, maintains the original interaction
	// direction, and does not block motion.
	
	interaction.m_supports       = true;
	interaction.m_blocks         = false;
	interaction.m_next_direction = interaction.m_direction;
	
	// Get player viewspace directions.

	Enigma::Location::Direction down = interaction.m_location.m_surface;
	Enigma::Location::Direction up   = Enigma::Location::get_opposite(down);

	if (interaction.m_direction == surface)
	{
		// The interaction direction is toward a LadderEnd surface.  If the
		// LadderEnd is rotated up, the player can climb up.  This is the only
		// orientation of a LadderEnd that blocks motion.
		
		if (rotation == up)
		{
			interaction.m_next_direction = up;
			interaction.m_blocks         = true;
		}
	}
	else if (interaction.m_direction == opposite_surface)
	{
		// The interaction direction is away from a LadderEnd surface.  If the
		// and LadderEnd rotation is down, the player can climb down.  For all
		// other LadderEnd rotations on the surface, the player can let go
		// (LadderEnd no longer provides support).
		
		if (rotation == down)
			interaction.m_next_direction = down;
		else
			interaction.m_supports = false;
	}
	else if (interaction.m_direction == opposite_rotation)
	{
		// The interaction direction runs opposite to a LadderEnd rotation.
		// The player can let go (LadderEnd no longer provides support).
		
		interaction.m_supports = false;
	}
}

//-------------------------------------------------------------
// This static method is called within an active OpenGL context
// to initialize class resources.
//-------------------------------------------------------------

void Enigma::LadderEnd::initialize()
{
	// Load object mesh data.

	active_mesh.load(active_name);
}
