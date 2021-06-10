// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Knife class implementation.  The Knife class describes
// a stainless steel knife object.
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
#include "Knife.h"
#include "Mesh.h"

//--------------------
// Local declarations.
//--------------------

static const std::string active_name = "Knife_A.msh";
static Enigma::Mesh active_mesh;

static const std::string thumbnail_name = "Knife.png";
static Glib::RefPtr<Gdk::Pixbuf> thumbnail_pixbuf;

static const std::string singular_name_string = _("Stainless Steel Knife");
static const std::string plural_name_string   = _("Stainless Steel Knives");

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::Knife::Knife()
{
	// Customize base class data.
	
	m_id = Enigma::Object::ID::KNIFE;
}

//----------------------------------------------------------------------------
// This method is called within an active OpenGL context to render the object.
//----------------------------------------------------------------------------
// viewer: Location of viewer in the world.
//----------------------------------------------------------------------------

void Enigma::Knife::render(const Enigma::FineLocation& viewer)
{		
	active_mesh.render(m_location, viewer);
}

//--------------------------------------------------------------------
// This is the default implementation of a virtual method that returns
// a thumbnail image of the object.
//--------------------------------------------------------------------
// RETURN: Thumbnail pixbuf.
//--------------------------------------------------------------------

Glib::RefPtr<Gdk::Pixbuf> Enigma::Knife::thumbnail()
{
	return thumbnail_pixbuf;
}

//--------------------------------------------------------------------
// This is the default implementation of a virtual method that returns
// the singular name of the object.
//--------------------------------------------------------------------
// RETURN: Singular name string.
//--------------------------------------------------------------------

const std::string& Enigma::Knife::singular_name()
{
	return singular_name_string;
}

//--------------------------------------------------------------------
// This is the default implementation of a virtual method that returns
// the plural name of the object.
//--------------------------------------------------------------------
// RETURN: Plural name string.
//--------------------------------------------------------------------

const std::string& Enigma::Knife::plural_name()
{
	return plural_name_string;
}

//-------------------------------------------------------------------
// This static method initializes class resources.  An OpenGL context
// must be active so the object meshes can be loaded.
//-------------------------------------------------------------------

void Enigma::Knife::initialize()
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
