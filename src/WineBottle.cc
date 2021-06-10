// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the WineBottle class implementation.   The WineBottle class
// describes a bottle of red wine object.
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
#include "WineBottle.h"
#include "Mesh.h"

//--------------------
// Local declarations.
//--------------------

static const std::string active_name = "WineBottle_A.msh";
static Enigma::Mesh active_mesh;

static const std::string thumbnail_name = "WineBottle.png";
static Glib::RefPtr<Gdk::Pixbuf> thumbnail_pixbuf;

static const std::string singular_name_string = _("Bottle of Wine");
static const std::string plural_name_string   = _("Bottles of Wine");

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::WineBottle::WineBottle()
{
	// Customize base class data.
	
	m_id = Enigma::Object::ID::WINEBOTTLE;
}

//----------------------------------------------------------------------------
// This method is called within an active OpenGL context to render the object.
//----------------------------------------------------------------------------
// viewer: Location of viewer in the world.
//----------------------------------------------------------------------------

void Enigma::WineBottle::render(const Enigma::FineLocation& viewer)
{		
	active_mesh.render(m_location, viewer);
}

//--------------------------------------------------------------------
// This is the default implementation of a virtual method that returns
// a thumbnail image of the object.
//--------------------------------------------------------------------
// RETURN: Thumbnail pixbuf.
//--------------------------------------------------------------------

Glib::RefPtr<Gdk::Pixbuf> Enigma::WineBottle::thumbnail()
{
	return thumbnail_pixbuf;
}

//--------------------------------------------------------------------
// This is the default implementation of a virtual method that returns
// the singular name of the object.
//--------------------------------------------------------------------
// RETURN: Singular name string.
//--------------------------------------------------------------------

const std::string& Enigma::WineBottle::singular_name()
{
	return singular_name_string;
}

//--------------------------------------------------------------------
// This is the default implementation of a virtual method that returns
// the plural name of the object.
//--------------------------------------------------------------------
// RETURN: Plural name string.
//--------------------------------------------------------------------

const std::string& Enigma::WineBottle::plural_name()
{
	return plural_name_string;
}

//-------------------------------------------------------------------
// This static method initializes class resources.  An OpenGL context
// must be active so the object meshes can be loaded.
//-------------------------------------------------------------------

void Enigma::WineBottle::initialize()
{
	// Assemble full resource path to the thumbnail image.
		
	std::string path = G_RESOURCE_PREFIX;
	path += thumbnail_name;
	
	// Load thumbnail image from the resources and create a PixBuf from it. 
	
	Gtk::Image image;
	image.set_from_resource(path);
	thumbnail_pixbuf = image.get_pixbuf();
	
	// Load object mesh data.

	active_mesh.load(active_name);
	
}
