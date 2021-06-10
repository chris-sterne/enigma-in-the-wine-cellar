// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the PlayerView class implementation.  The PlayerView class
// displays the player's location and visible surroundings within a game map.
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
#include <glibmm/main.h>
#include "Mesh.h"
#include "Sound.h"
#include "Player.h"
#include "ItemList.h"
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
#include "PlayerView.h"

//-------------------
// Local declarations
//-------------------

static const std::string ui_name = "PlayerView.ui";

static const std::string startup_message =
	_("\nEntering the dark passage...");

// Indoor background colour (dark grey).

static const GLfloat INDOOR_R = 0.156f;
static const GLfloat INDOOR_G = 0.156f;
static const GLfloat INDOOR_B = 0.156f;

// Startup display duration (milliseconds).

static const int STARTUP_DELAY = 1500;

// Table for converting a world direction into a text string.

static const Glib::ustring
	direction_to_text[(int)Enigma::Location::Direction::TOTAL]
{
  _("None"),
  _("North"),
  _("South"),
  _("East"),
  _("West"),
  _("Above"),
  _("Below"),
  _("Center"),
};

//---------------------------------
// This is the default constructor. 
//---------------------------------

Enigma::PlayerView::PlayerView()
{
	// Build the user interface from its Glade file.

	std::string path = G_RESOURCE_PREFIX;
	path += ui_name;
	
	m_builder = Gtk::Builder::create_from_resource(path);
	
	// Add the top-level box of the user interface in the builder
	// to the PlayerView box (creates a box within a box).
	
	Gtk::Box* box = nullptr;
	m_builder->get_widget("parent_box", box);	
	this->pack_start(*box, true, true, 0);
	
	// Cache pointers to some user interface widgets.
	
	m_location = nullptr;
	m_builder->get_widget("location_label", m_location);
	
	m_area = nullptr;
	m_builder->get_widget("view_area", m_area);

	// Connect view area "resize" signal to UserAction object so it can adjust
	// its layout accordingly.

	m_area->signal_resize()
		.connect(sigc::mem_fun(m_user_action, &Enigma::UserAction::resize));

	// Show a startup message in place of the player's location.
	
	m_location->set_text(startup_message);

	// Connect to the view area realize signal that will begin the startup
	// procedure.
	
	m_area->signal_realize()
		.connect(sigc::mem_fun(this, &PlayerView::on_realize_area));
}

//--------------------------------------------------------------
// This method is called when the view widget is being realized.
//--------------------------------------------------------------

void Enigma::PlayerView::on_realize_area()
{
	// Start a delay timer for showing the startup display.
	
	Glib::signal_timeout()
		.connect_once(sigc::mem_fun(this, &Enigma::PlayerView::on_startup_done),
		              STARTUP_DELAY,
		              Glib::PRIORITY_DEFAULT);	              
	
	// Initialize classes.  Some classes require a sound server to be running
	// before they are initialized.  The OpenGL context provided by the view
	// area is needed to initialize the Mesh class and other classes that
	// render meshes.
	
	m_area->make_current();
	
	Enigma::Sound::initialize();
	Enigma::Mesh::initialize();
	Enigma::Object::initialize();
	Enigma::ItemList::initialize();
	Enigma::Player::initialize();
	Enigma::WineCellar::initialize();
	Enigma::BlockWall::initialize();
	Enigma::StoneWall::initialize();
	Enigma::Ladder::initialize();
	Enigma::LadderEnd::initialize();
	Enigma::Blocker::initialize();
	Enigma::Mover::initialize();
	Enigma::Turner::initialize();
	Enigma::Mushrooms::initialize();
	Enigma::CorkScrew::initialize();
	Enigma::WineBottle::initialize();
	Enigma::Bread::initialize();
	Enigma::Cheese::initialize();
	Enigma::Knife::initialize();
	Enigma::Apple::initialize();
	Enigma::WineGlass::initialize();
	Enigma::PlaceMat::initialize();
	Enigma::Grapes::initialize();
	Enigma::Skull::initialize();
	Enigma::EdgeDrain::initialize();
	Enigma::AirVent::initialize();
	
	// Connect to the view area render signal so the startup display will be
	// rendered.  The returned connection is saved so it can be disconnected
	// when the startup display is not to be shown.
		
	m_connection_render = m_area->signal_render()
		.connect(sigc::mem_fun(this, &Enigma::PlayerView::render_startup));
}

//---------------------------------------------------------
// This method renders a startup display.
//---------------------------------------------------------
// context: Rendering GLContext.
// RETURN:  TRUE to stop other handlers from being invoked.
//---------------------------------------------------------

bool Enigma::PlayerView::render_startup(
	const Glib::RefPtr<Gdk::GLContext>& context)
{
	// Clear the view with a background colour.

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(INDOOR_R, INDOOR_G, INDOOR_B, -1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Prepare a WineCellar object to be rendered.
	
	std::shared_ptr<Enigma::Object> cellar =
		Enigma::Object::create(Enigma::Object::ID::WINECELLAR);
	
	cellar->m_location.set(0, 0, 0,
	                       Enigma::Location::Direction::BELOW,
	                       Enigma::Location::Direction::NORTH);
	
	// Position the viewer so the WineCellar object fills the view,
	// then render the object.
	
	Enigma::FineLocation viewer(cellar->m_location);
	viewer.m_fine_north        = 0.8f;
	viewer.m_fine_rotate_above = 180.0f;
	cellar->render(viewer);
	
	return true;
}

//--------------------------------------------------------
// This method is called when the startup delay has ended.
//--------------------------------------------------------

void Enigma::PlayerView::on_startup_done()
{		
	// Pass mouse button events to UserAction object for conversion into player
	// motion actions.
	
	m_area->add_events(Gdk::BUTTON_PRESS_MASK);

	m_area->signal_button_press_event()
		.connect(sigc::mem_fun(m_user_action, &Enigma::UserAction::button_press));

	// Emit "startup done" signal to indicate startup has completed.
	
	m_signal_startup_done.emit();
}

//--------------------------------------------------------------
// This method establishes signal connections with a new player,
// disconnecting signals to a previous player.
//--------------------------------------------------------------
// player: Active player.
//--------------------------------------------------------------

void Enigma::PlayerView::set_player(std::shared_ptr<Enigma::Player>& player)
{	
	// Connect PlayerView view "render" signal to the new player, replacing
	// an existing connection.  This signal informs the player to render
	// its view.
	
	m_connection_render.disconnect();
	
	m_connection_render = m_area->signal_render()
		.connect(sigc::mem_fun(*player, &Enigma::Viewer::render));

	// Connect UserAction "action" signal to the new player, replacing
	// an existing connection.  This signal is emitted when input occurs
	// in the user interface.
	
	m_connection_action.disconnect();
	
	m_connection_action = m_user_action.signal_action()
		.connect(sigc::mem_fun(*player, &Enigma::Player::queue_action));
	
	// Connect player "queue render" signal to the new player, replacing
	// an existing connection.  This signal is emitted by a player to queue
	// a rendering of its view.
	
	m_connection_queue_render.disconnect();
	
	m_connection_queue_render = player->signal_queue_render()
		.connect(sigc::mem_fun(m_area, &Gtk::GLArea::queue_render));

	// Connect player "location" signal to the new player, replacing
	// an existing connection.  This signal is emitted by a player when
	// it changes location.  Trigger an emission of this signal to update
	// the location display.
	
	m_connection_location.disconnect();
	
	m_connection_location = player->signal_location()
		.connect(sigc::mem_fun(*this, &Enigma::PlayerView::set_location));

	player->do_location();
	
	// Send a HOLD_STILL action to the player so it starts playing.
	
	player->queue_action(Enigma::Player::Action::HOLD_STILL);
}

//-------------------------------------------------
// This method updates the player location display.
//-------------------------------------------------

void Enigma::PlayerView::set_location(const Enigma::Location& location)
{	
	// Convert the rotation into text, ensuring the rotation value will not
	// overrun the direction text array. 
	
	Glib::ustring rotation_text;
	
	if (location.m_rotation < Enigma::Location::Direction::TOTAL)
		rotation_text = direction_to_text[(int)location.m_rotation];
	else
		rotation_text = direction_to_text[(int)Enigma::Location::Direction::NONE];
	
	// Compose a text string containing all location information, and set
	// this composed string in the location display.

	Glib::ustring text =
		Glib::ustring::compose(_("East %1, North %2, Above %3\n%4"),
		                       location.m_east,
		                       location.m_north,
		                       location.m_above,
		                       rotation_text);

	m_location->set_text(text);
}

//------------------------------------------------------
// This method returns the "startup done" signal server.
//------------------------------------------------------

Enigma::PlayerView::type_signal_startup_done
	Enigma::PlayerView::signal_startup_done()
{
  return m_signal_startup_done;
}
