// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Location class implementation.  The Location class
// describes the location of an object within a game world.
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

#include "Location.h"

//-------------------
// Local declarations
//-------------------

// Assignments to shorten world space viewing directions.

static const Enigma::Location::Direction WNONE =
	Enigma::Location::Direction::NONE;

static const Enigma::Location::Direction WNORTH =
	Enigma::Location::Direction::NORTH;

static const Enigma::Location::Direction WSOUTH =
	Enigma::Location::Direction::SOUTH;

static const Enigma::Location::Direction WEAST =
	Enigma::Location::Direction::EAST;

static const Enigma::Location::Direction WWEST =
	Enigma::Location::Direction::WEST;

static const Enigma::Location::Direction WABOVE =
	Enigma::Location::Direction::ABOVE;

static const Enigma::Location::Direction WBELOW =
	Enigma::Location::Direction::BELOW;

static const Enigma::Location::Direction WCENTER =
	Enigma::Location::Direction::CENTER;

static const int WTOTAL = 8;

// This table provides next rotation (direction) when turning on a surface.
//
// Turn Left: Set the first index to the current surface, and the second
// index to the current rotation on the surface.  Read the new rotation
// from the table.
//
// Turn Right: Set the first index to the current rotation on the surface,
// and the second index to the current surface.  Read the new rotation
// from the table.
//
// Unsupported rotations on surfaces are assigned WNONE to indicate invalid
// surface and rotation combinations.

static const Enigma::Location::Direction turn_table[WTOTAL][WTOTAL] =
//-----------------------------------------------------------------
//  None   North   South   East    West    Above   Below   Center
//-----------------------------------------------------------------
{
	{WNONE, WNONE,  WNONE,  WNONE,  WNONE,  WNONE,  WNONE,  WNONE  },
	{WNONE, WNONE,  WNONE,  WABOVE, WBELOW, WWEST,  WEAST,  WCENTER},
	{WNONE, WNONE,  WNONE,  WBELOW, WABOVE, WEAST,  WWEST,  WCENTER},
	{WNONE, WBELOW, WABOVE, WNONE,  WNONE,  WNORTH, WSOUTH, WCENTER},
	{WNONE, WABOVE, WBELOW, WNONE,  WNONE,  WSOUTH, WNORTH, WCENTER},
	{WNONE, WEAST,  WWEST,  WSOUTH, WNORTH, WNONE,  WNONE,  WCENTER},
	{WNONE, WWEST,  WEAST,  WNORTH, WSOUTH, WNONE,  WNONE,  WCENTER},
	{WNONE, WNORTH, WSOUTH, WEAST,  WWEST,  WABOVE, WBELOW, WCENTER}
};

// This array contains opposite directions.

static const Enigma::Location::Direction opposite_table[WTOTAL] =
//-----------------------------------------------------------------
//  None   North   South   East    West    Above   Below   Center
//-----------------------------------------------------------------
{
   WNONE, WSOUTH, WNORTH, WWEST, WEAST, WBELOW, WABOVE, WCENTER
};

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::Location::Location()
{
	// Initialize instance variables.
	
	clear();
}

//----------------------------------------
// This method clears all location values.
// ---------------------------------------

void Enigma::Location::clear()
{
	// Clear all room position values.
	
	m_east  = 0;
	m_north = 0;
	m_above = 0;
	
	// Set a default surface and rotation on this surface.
	
	m_surface  = Enigma::Location::Direction::NORTH;
	m_rotation = Enigma::Location::Direction::ABOVE;
}

//-----------------------------------------------------
// This method sets all location values from arguments.
//-----------------------------------------------------
// east:     East position
// above:    Above position
// north:    North position
// surface:  Surface
// rotation: Rotation on surface
//-----------------------------------------------------

void Enigma::Location::set(guint16 east,
                           guint16 above,
                           guint16 north,
                           Enigma::Location::Direction surface,
                           Enigma::Location::Direction rotation)
{
	m_east     = east;
	m_above    = above;
	m_north    = north;
	m_surface  = surface;
	m_rotation = rotation;
}

//---------------------------------------------------------------
// This static method returns the opposite world space direction.
// --------------------------------------------------------------
// direction: Original direction.
// RETURN:    Opposite direction.
//---------------------------------------------------------------

Enigma::Location::Direction
Enigma::Location::get_opposite(Enigma::Location::Direction direction)
{
	short index = short(direction);
	
	// Return the opposite direction if the index falls within the table.
	
	if (index < WTOTAL)
		return opposite_table[index];
	else
		return direction;
}

//-----------------------------------------------------------------
// This static method returns the world space direction to the left
// in view space.
// ----------------------------------------------------------------
// surface:  Surface direction.
// rotation: Rotation of surface.
// RETURN:   World direction to the left.
//-----------------------------------------------------------------

Enigma::Location::Direction
Enigma::Location::get_left(Enigma::Location::Direction surface,
                           Enigma::Location::Direction rotation)
{
	short surface_index  = short(surface);
	short rotation_index = short(rotation);

	// Return the left direction if the indices fall within the table.
	
	if ((surface_index < WTOTAL) && (rotation_index < WTOTAL))
		return turn_table[surface_index][rotation_index];
	else
		return WNONE;
}

//------------------------------------------------------------------
// This static method returns the world space direction to the right
// in view space. 
// -----------------------------------------------------------------
// surface:  Surface direction.
// rotation: Rotation of surface.
// RETURN:   World direction to the right.
//------------------------------------------------------------------

Enigma::Location::Direction
Enigma::Location::get_right(Enigma::Location::Direction surface,
                            Enigma::Location::Direction rotation)
{
	short surface_index  = short(surface);
	short rotation_index = short(rotation);

	// Return the right direction if the indices fall within the table.
	
	if ((surface_index < WTOTAL) && (rotation_index < WTOTAL))
		return turn_table[rotation_index][surface_index];
	else
		return WNONE;
}
