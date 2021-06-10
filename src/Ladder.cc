// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Ladder class implementation.  A Ladder is an object that
// supports a player and allows climbing up or down it.
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
 
#include "Ladder.h"
#include "Mesh.h"

//--------------------
// Local declarations.
//--------------------

static const std::string active_name = "Ladder_A.msh";
static Enigma::Mesh active_mesh;

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::Ladder::Ladder()
{
	// Customize base class data.
	
	m_id = Enigma::Object::ID::LADDER;
}

//----------------------------------------------------------------------------
// This method is called within an active OpenGL context to render the object.
//----------------------------------------------------------------------------
// location: Location of viewer in the world.
//----------------------------------------------------------------------------

void Enigma::Ladder::render(const Enigma::FineLocation& location)
{
	active_mesh.render(m_location, location);
}

//---------------------------------------
// This method interacts with the object.
//---------------------------------------
// interaction: Interaction data.
//---------------------------------------

void Enigma::Ladder::interact(Enigma::Interaction& interaction)
{
	// Get Ladder orientation.
	
	Enigma::Location::Direction surface  = m_location.m_surface;
	Enigma::Location::Direction rotation = m_location.m_rotation;
	
	Enigma::Location::Direction opposite_surface =
		Enigma::Location::get_opposite(surface);
		
	Enigma::Location::Direction opposite_rotation =
		Enigma::Location::get_opposite(rotation);
	
	// Determine interaction zone based where the Ladder is located
	// along the interaction direction.

	if (interaction.m_direction == surface)
		interaction.m_zone = 2;
	else if (interaction.m_direction == opposite_surface)
		interaction.m_zone = -2;
	else
		interaction.m_zone = 0;
	
	// Determine the effects of a Ladder interaction with player. In general,
	// a Ladder provides support, maintains the original interaction
	// direction, and does not block motion.
	
	interaction.m_supports       = true;
	interaction.m_blocks         = false;
	interaction.m_next_direction = interaction.m_direction;

	// Get player viewspace directions.

	Enigma::Location::Direction down = interaction.m_location.m_surface;
	Enigma::Location::Direction up   = Enigma::Location::get_opposite(down);

	if (interaction.m_direction == surface)
	{
		// The interaction direction is toward the surface of a Ladder.  If the
		// ladder is oriented vertically, the player can climb up.  This is the
		// only orientation of a Ladder that blocks motion.

		if ((rotation == up) || (rotation == down))
		{
			interaction.m_next_direction = up;
			interaction.m_blocks         = true;
		}
	}
	else if (interaction.m_direction == opposite_surface)
	{
		// The interaction direction is away from the surface of a Ladder.
		// If the ladder is vertical, the player can climb down.  Otherwise,
		// the ladder is horizontal and player can let go (Ladder no longer
		// provides support).
		
		if ((rotation == up) || (rotation == down))
			interaction.m_next_direction = down;
		else
			interaction.m_supports = false;
	}
}

//-------------------------------------------------------------------
// This static method initializes class resources.  An OpenGL context
// must be active so the object meshes can be loaded.
//-------------------------------------------------------------------

void Enigma::Ladder::initialize()
{
	// Load object mesh data.

	active_mesh.load(active_name);
}
