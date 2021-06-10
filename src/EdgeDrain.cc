// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the EdgeDrain class implementation.  The EdgeDrain class
// is a water drain at the edge of a wall.
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
 
#include "EdgeDrain.h"
#include "Mesh.h"

//--------------------
// Local declarations.
//--------------------

static const std::string active_name = "EdgeDrain_A.msh";
static Enigma::Mesh active_mesh;

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::EdgeDrain::EdgeDrain()
{
	// Customize base class data.
	
	m_id = Enigma::Object::ID::EDGEDRAIN;
}

//------------------------------------------------------
// This method is called within an active OpenGL context
// to render the object.
//------------------------------------------------------
// viewer: Location of viewer in the world.
//------------------------------------------------------

void Enigma::EdgeDrain::render(const Enigma::FineLocation& viewer)
{
	active_mesh.render(m_location, viewer);
}

//-------------------------------------------------------------------
// This static method initializes class resources.  An OpenGL context
// must be active so the object meshes can be loaded.
//-------------------------------------------------------------------

void Enigma::EdgeDrain::initialize()
{
	// Load object mesh data.

	active_mesh.load(active_name);
}
