// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the HelpView class implementation.  The HelpView class displays
// the game's objective and instructions.
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

#include <gtkmm/textview.h>
#include <gtkmm/button.h>
#include "HelpView.h"

//--------------------
// Local declarations.
//--------------------

static const std::string ui_name   = "HelpView.ui";
static const std::string text_name = "HelpView.txt";

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::HelpView::HelpView()
{	
	// Build the user interface from its Glade file.

	std::string path = G_RESOURCE_PREFIX;
	path += ui_name;
	
	m_builder = Gtk::Builder::create_from_resource(path);

	// Add the top-level box of the user interface in the builder
	// to the HelpView box (a box within a box).
	
	Gtk::Box* box = nullptr;
	m_builder->get_widget("parent_box", box);	
	this->pack_start(*box, true, true, 0);

	// Extract the help text from the resource bundle.

	path = G_RESOURCE_PREFIX;
	path += text_name;
	
	Glib::RefPtr<const Glib::Bytes> bytes = 
		Gio::Resource::lookup_data_global(path, Gio::RESOURCE_LOOKUP_FLAGS_NONE);

 	gsize size;
	gchar* text = (gchar*)bytes->get_data(size);

	// Copy the help text to the TextView buffer.

	Gtk::TextView* textview = nullptr;
	m_builder->get_widget("text_viewer", textview);
 		
	Glib::RefPtr<Gtk::TextBuffer> buffer = textview->get_buffer();
	buffer->set_text(text);
	
	// Connect to the Done button clicked signal, which will allow the HelpView
	// widget to emit a "Done" signal. 
	
	Gtk::Button* button = nullptr;
	m_builder->get_widget("ok_button", button);
	
	button->signal_clicked().connect(sigc::mem_fun(*this, &HelpView::on_done));
}

//-------------------------------------------------
// This method returns the view done signal server.
//-------------------------------------------------

Enigma::HelpView::type_signal_done Enigma::HelpView::signal_done()
{
  return m_signal_done;
}

//----------------------------------------------------
// This method is called the "Done" button is pressed.
//----------------------------------------------------

void Enigma::HelpView::on_done()
{
	// Emit the "view done" signal.

  m_signal_done.emit();
}
