// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the WineCellar class implementation.  The WineCellar class
// is a wine cellar shelf that marks the entrance to the dark passage at
// the beginning of a game.
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
 
#include "WineCellar.h"
#include "Mesh.h"

//--------------------
// Local declarations.
//--------------------

static const std::string active_name = "WineCellar_A.msh";
static Enigma::Mesh active_mesh;

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::WineCellar::WineCellar()
{
	// Customize base class data.
	
	m_id = Enigma::Object::ID::WINECELLAR;
}

//------------------------------------------------------
// This method is called within an active OpenGL context
// to render the object.
//------------------------------------------------------
// viewer: Location of viewer in the world.
//------------------------------------------------------

void Enigma::WineCellar::render(const Enigma::FineLocation& viewer)
{
	active_mesh.render(m_location, viewer);
}

//-------------------------------------------------------------------
// This static method initializes class resources.  An OpenGL context
// must be active so the object meshes can be loaded.
//-------------------------------------------------------------------

void Enigma::WineCellar::initialize()
{
	// Load object mesh data.

	active_mesh.load(active_name);
}
