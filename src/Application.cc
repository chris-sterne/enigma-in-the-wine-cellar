// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Application class implementation.  The Application class
// serves as a foundation on which the game is constructed.
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

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include "Application.h"

//-------------------
// Local declarations
//-------------------

static const std::string ui_name = "Application.ui";

//---------------------------------------------------------------------------
// This method is the constructor.  An application ID is passed to the parent
// class constructor.
//---------------------------------------------------------------------------

Enigma::Application::Application() : Gtk::Application(G_APPLICATION_ID)
{
}

//---------------------------------------------------------------------------
// This method overrides the base class on_activate() method.  Part of the
// user interface and application logic is assembled before a startup display
// is shown.
//---------------------------------------------------------------------------

void Enigma::Application::on_activate()
{		
	// Build the user interface from its Glade UI description. 
	
	std::string path = G_RESOURCE_PREFIX;
	path += ui_name;
	
	m_builder = Gtk::Builder::create_from_resource(path);

	// Ensure all view pages will be visible when shown.

	m_playerview.show();
	m_itemsview.show();
	m_worldsview.show();
	m_helpview.show();
	m_aboutview.show();
	
	// Add view pages to the pager.

	m_view_pager = nullptr;
	m_builder->get_widget("view_pager", m_view_pager);

	m_view_pager->add(m_playerview);
	m_view_pager->add(m_itemsview);
	m_view_pager->add(m_worldsview);
	m_view_pager->add(m_helpview);
	m_view_pager->add(m_aboutview);
	
	// Add window containing the user interface to the application.
	
	Gtk::Window* window = nullptr;
	m_builder->get_widget("main_window", window);	
	add_window(*window);

	// Create a signal link to quit the game if the "Quit" button is clicked.
	
	Gtk::Button* button = nullptr;
	m_builder->get_widget("quit_button", button);
	
	button->signal_clicked()
		.connect(sigc::mem_fun(this, &Application::quit));

	// The Giomm 2.4 headers do not define a virtual on_shutdown() method that
	// can be overridden, so attach a signal handler to the shutdown signal.
		
	signal_shutdown()
		.connect(sigc::mem_fun(this, &Application::do_shutdown));

	// Connect to the PlayerView "startup done" signal.  This signal indicates
	// classes have been initialized, and a game world can be loaded.

	m_playerview.signal_startup_done()
		.connect(sigc::mem_fun(this, &Application::on_activate_finalize));

	m_view_pager->set_visible_child(m_playerview);
}

//--------------------------------------------------------------------------
// This method is called when the PlayerView startup is done.  The remaining
// user interface and application logic is now assembled. 
//--------------------------------------------------------------------------

void Enigma::Application::on_activate_finalize()
{	
	// Create a signal link to the restart dialog response.

	m_restart_dialog = nullptr;
	m_builder->get_widget("restart_dialog", m_restart_dialog);
		
	m_restart_dialog->signal_response()
		.connect(sigc::mem_fun(*this, &Enigma::Application::on_restart_response));
	
	// Create a signal link to show a dialog if the game "Restart"
	// button is clicked.
	
	Gtk::Button* button = nullptr;
	m_builder->get_widget("restart_button", button);
	
	button->signal_clicked()
		.connect(sigc::mem_fun(m_restart_dialog, &Gtk::Dialog::show));

	// Create a signal link to show the item view if the "Items"
	// button is clicked.
	
	button = nullptr;
	m_builder->get_widget("items_button", button);
	
	button->signal_clicked()
		.connect(sigc::mem_fun(this, &Application::show_items));

	// Create a signal link to show the worlds view if the "Worlds" button
	// is clicked.
	
	button = nullptr;
	m_builder->get_widget("worlds_button", button);
	
	button->signal_clicked()
		.connect(sigc::mem_fun(this, &Application::show_worlds));

	// Create a signal link to show the game help view if the "Help" button
	// is clicked.
	
	button = nullptr;
	m_builder->get_widget("help_button", button);
	
	button->signal_clicked()
		.connect(sigc::mem_fun(this, &Application::show_help));

	// Create a signal link to show the game help view if the "About" button
	// is clicked.
	
	button = nullptr;
	m_builder->get_widget("about_button", button);
	
	button->signal_clicked()
		.connect(sigc::mem_fun(this, &Application::show_about));

	// Signal to return to the player view after viewing items.
	
	m_itemsview.signal_done()
		.connect(sigc::mem_fun(this, &Application::show_player));

	// Signal to return to the player view after viewing worlds.
	
	m_worldsview.signal_cancel()
		.connect(sigc::mem_fun(this, &Application::show_player));

	// Signal to load a different game world.
	
	m_worldsview.signal_play()
		.connect(sigc::mem_fun(this, &Application::load_world));

	// Signal to return to the player view after viewing help.
	
	m_helpview.signal_done()
		.connect(sigc::mem_fun(this, &Application::show_player));

	// Signal to return to the player view after viewing information
	// about the application.
	
	m_aboutview.signal_done()
		.connect(sigc::mem_fun(this, &Application::show_player));

	// Create an empty game world.

	m_world = std::make_shared<Enigma::World>();

	// Connect the world "remains" signal to the worlds view to keep
	// the total remaining items updated for the active world.
	
	m_world->items()->signal_remains().connect(
		sigc::mem_fun(m_worldsview, &Enigma::WorldsView::set_item_remains));

	// Connect the world "player" signal to the player view so it can
	// establish connections with the active player.

	m_world->players()->signal_player().connect(
		sigc::mem_fun(m_playerview, &Enigma::PlayerView::set_player));

	// Connect the world "player" signal to the items view so it can
	// establish connections with the active player.

	m_world->players()->signal_player().connect(
		sigc::mem_fun(m_itemsview, &Enigma::ItemsView::set_player));

	// Set the initial game world to be played.  The worlds view will emit
	// the filename in a signal if it exists in its records.  Later versions
	// of this application will set the last played game from a configuration
	// file.
	
	m_worldsview.set_world("worlds/first.ewc");
}

//------------------------------------------
// This method loads the game world to play.
//------------------------------------------
// filename: Game world file to be played.
//------------------------------------------

void Enigma::Application::load_world(const std::string& filename)
{
	// Save the old world using its internal filename.

	m_world->save("");

	// Load and start playing the new world.
	
	m_world->load(filename);
	show_player();
}

//-------------------------------------------------
// Method called when application is shutting down.
//-------------------------------------------------

void Enigma::Application::do_shutdown()
{
	// Save world before shutting down.
	
	m_world->save("");
}

//-------------------------------------------------------------------
// This method handles response from the restart confirmation dialog.
//-------------------------------------------------------------------
// response_id: Response ID from dialog.
//-------------------------------------------------------------------

void Enigma::Application::on_restart_response(int response_id)
{	
	// Start playing the game world from its "restart" state
	// if the restart dialog "OK" button is pressed.
	
	if (response_id == Gtk::RESPONSE_OK)
	{
		m_world->restart();
		show_player();
	}
	
	// Hide the restart dialog.
	
	m_restart_dialog->hide();
}

//-------------------------------------
// This method displays the items view.
//-------------------------------------

void Enigma::Application::show_items()
{
	m_view_pager->set_visible_child(m_itemsview);
}

//--------------------------------------
// This method displays the worlds view.
//--------------------------------------

void Enigma::Application::show_worlds()
{
	m_view_pager->set_visible_child(m_worldsview);
}

//-----------------------------------------
// This method displays the game help view.
//-----------------------------------------

void Enigma::Application::show_help()
{
	m_view_pager->set_visible_child(m_helpview);
}

//----------------------------------------------
// This method displays application information.
//----------------------------------------------

void Enigma::Application::show_about()
{
	m_view_pager->set_visible_child(m_aboutview);
}

//--------------------------------------
// This method displays the player view.
//--------------------------------------

void Enigma::Application::show_player()
{
	m_view_pager->set_visible_child(m_playerview);
}
