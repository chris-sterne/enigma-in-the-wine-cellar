// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Item class implementation. The Item class describes an
// object that can be found by a player.
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

#include "Item.h"

//-------------------------------------------------------------------
// This constructor has an Object ID argument which is used to create
// the Object being turned into an item.
//-------------------------------------------------------------------
// id: Object ID.
//-------------------------------------------------------------------

Enigma::Item::Item(Enigma::Object::ID id)
{
	// Create object to act as an item.

	m_object = Enigma::Object::create(id);
	
	// Initialize other instance data.

	m_category         = Enigma::Item::Category::NONE;
	m_active           = true;
	m_owner            = 0;
	m_selected         = false;
	m_used             = false;
	m_active_saved     = m_active;
	m_owner_saved      = m_owner;
	m_selected_saved   = m_selected;
	m_used_saved       = m_used;
	m_active_restart   = m_active;
	m_owner_restart    = m_owner;
	m_selected_restart = m_selected;
	m_used_restart     = m_used;
}

//--------------------------------------------------
// This method restores item to its "restart" state.
//--------------------------------------------------

void Enigma::Item::restart()
{
	m_active   = m_active_restart;
	m_owner    = m_owner_restart;
	m_selected = m_selected_restart;
	m_used     = m_used_restart;
}

//--------------------------------------------------------------------------
// This method is called within an active OpenGL context to render the item.
//--------------------------------------------------------------------------
// viewer: Location of viewer in the world.
//--------------------------------------------------------------------------

void Enigma::Item::render(const Enigma::FineLocation& viewer)
{
	m_object->render(viewer);
}

//-------------------------------------
// This method interacts with the item.
//-------------------------------------
// interaction: Interaction data.
//-------------------------------------

void Enigma::Item::interact(Enigma::Interaction& interaction)
{
}
