// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Object class implementation.  The Object class is the base
// class of all objects within a game world.
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

#include <glibmm/i18n.h>
#include <gtkmm/image.h>
#include "WineCellar.h"
#include "BlockWall.h"
#include "StoneWall.h"
#include "Ladder.h"
#include "LadderEnd.h"
#include "Blocker.h"
#include "Mover.h"
#include "Turner.h"
#include "Mushrooms.h"
#include "CorkScrew.h"
#include "WineBottle.h"
#include "Bread.h"
#include "Cheese.h"
#include "Knife.h"
#include "Apple.h"
#include "WineGlass.h"
#include "PlaceMat.h"
#include "Grapes.h"
#include "Skull.h"
#include "EdgeDrain.h"
#include "AirVent.h"

//--------------------
// Local declarations.
//--------------------

static const std::string thumbnail_name = "None.png";
static Glib::RefPtr<Gdk::Pixbuf> thumbnail_pixbuf;

static const std::string singular_name_string =_("unknown item");
static const std::string plural_name_string   =_("unknown items");

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::Object::Object()
{
	// Initialize instance data.
	
	m_id = Enigma::Object::ID::NONE;
}

//--------------------------------------------------------------------
// This is the default implementation of a virtual method that returns
// a thumbnail image of the object.
//--------------------------------------------------------------------
// RETURN: Thumbnail pixbuf.
//--------------------------------------------------------------------

Glib::RefPtr<Gdk::Pixbuf> Enigma::Object::thumbnail()
{
	return thumbnail_pixbuf;
}

//--------------------------------------------------------------------
// This is the default implementation of a virtual method that returns
// the singular name of the object.
//--------------------------------------------------------------------
// RETURN: Singular name string.
//--------------------------------------------------------------------

const std::string& Enigma::Object::singular_name()
{
	return singular_name_string;
}

//--------------------------------------------------------------------
// This is the default implementation of a virtual method that returns
// the plural name of the object.
//--------------------------------------------------------------------
// RETURN: Plural name string.
//--------------------------------------------------------------------

const std::string& Enigma::Object::plural_name()
{
	return plural_name_string;
}

//---------------------------------------------------------------------
// This is the default implementation of a virtual method called within
// an active OpenGL context to render the object.
//---------------------------------------------------------------------
// viewer: Location of viewer in the world.
//---------------------------------------------------------------------

void Enigma::Object::render(const Enigma::FineLocation& viewer)
{
	// Override to render something interesting.
}

//-------------------------------------------------------------------
// This is the default implementation of a virtual method to interact
// with the object.
//-------------------------------------------------------------------
// interaction: Interaction data.
//-------------------------------------------------------------------

void Enigma::Object::interact(Enigma::Interaction& interaction)
{
	// Provide default information to the player. 
	
	interaction.m_zone           = 0;
	interaction.m_supports       = false;
	interaction.m_blocks         = false;
	interaction.m_next_direction = interaction.m_direction;
	interaction.m_next_rotation  = interaction.m_location.m_rotation;
}

//------------------------------------------------------
// This static method is a factory for creating objects.
//------------------------------------------------------
// id:     ID of object to be created.
// RETURN: Shared pointer to new object.
//------------------------------------------------------

std::shared_ptr<Enigma::Object>
  Enigma::Object::create(Enigma::Object::ID id)
{
	switch (id)
	{
		case Object::ID::WINECELLAR:
			return std::make_shared<Enigma::WineCellar>();
	
		case Object::ID::BLOCKWALL:
			return std::make_shared<Enigma::BlockWall>();
			
		case Object::ID::STONEWALL:
			return std::make_shared<Enigma::StoneWall>();
			
		case Object::ID::LADDER:
			return std::make_shared<Enigma::Ladder>();
			
		case Object::ID::LADDEREND:
			return std::make_shared<Enigma::LadderEnd>();

		case Object::ID::BLOCKER:
			return std::make_shared<Enigma::Blocker>();
			
		case Object::ID::MOVER:
			return std::make_shared<Enigma::Mover>();

		case Object::ID::TURNER:
			return std::make_shared<Enigma::Turner>();
		
		case Object::ID::MUSHROOMS:
			return std::make_shared<Enigma::Mushrooms>();
			
		case Object::ID::CORKSCREW:
			return std::make_shared<Enigma::CorkScrew>();
			
		case Object::ID::WINEBOTTLE:
			return std::make_shared<Enigma::WineBottle>();

		case Object::ID::BREAD:
			return std::make_shared<Enigma::Bread>();
			
		case Object::ID::CHEESE:
			return std::make_shared<Enigma::Cheese>();

		case Object::ID::KNIFE:
			return std::make_shared<Enigma::Knife>();

		case Object::ID::APPLE:
			return std::make_shared<Enigma::Apple>();
			
		case Object::ID::WINEGLASS:
			return std::make_shared<Enigma::WineGlass>();
			
		case Object::ID::PLACEMAT:
			return std::make_shared<Enigma::PlaceMat>();

		case Object::ID::GRAPES:
			return std::make_shared<Enigma::Grapes>();

		case Object::ID::SKULL:
			return std::make_shared<Enigma::Skull>();

		case Object::ID::EDGEDRAIN:
			return std::make_shared<Enigma::EdgeDrain>();
			
		case Object::ID::AIRVENT:
			return std::make_shared<Enigma::AirVent>();
		
		// Create a base Enigma::Object if the ID is unrecognized.
		
		default:
			return std::make_shared<Enigma::Object>();
	}
}

//-------------------------------------------------------------
// This static method is called within an active OpenGL context
// to initialize class resources.
//-------------------------------------------------------------

void Enigma::Object::initialize()
{	
	// Assemble full resource path to a default thumbnail image for an object.
		
	std::string path = G_RESOURCE_PREFIX;
	path += thumbnail_name;
	
	// Load thumbnail image from the resources and create a PixBuf from it.
	
	Gtk::Image image;
	image.set_from_resource(path);
	thumbnail_pixbuf = image.get_pixbuf();
}
