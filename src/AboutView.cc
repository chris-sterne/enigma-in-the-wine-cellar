// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the AboutView class implementation.  The AboutView class
// displays information about the application (name, version, author, etc.). 
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
#include <gtkmm/textview.h>
#include <gtkmm/button.h>
#include "AboutView.h"

//--------------------
// Local declarations.
//--------------------

static const std::string ui_name   = "AboutView.ui";
static const std::string text_name = "AboutView.txt";

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::AboutView::AboutView()
{
	// Build the user interface from its Glade file. 

	std::string path = G_RESOURCE_PREFIX;
	path += ui_name;

	m_builder = Gtk::Builder::create_from_resource(path);
  
	// Add the top-level box of the user interface in the builder
	// to the AboutView box (a box within a box).
	
	Gtk::Box* box = nullptr;
	m_builder->get_widget("parent_box", box);	
	this->pack_start(*box, true, true, 0);
  
	// Extract the About text from the resource bundle.

	path = G_RESOURCE_PREFIX;
	path += text_name;
	
	Glib::RefPtr<const Glib::Bytes> bytes = 
		Gio::Resource::lookup_data_global(path, Gio::RESOURCE_LOOKUP_FLAGS_NONE);

 	gsize size;
	gchar* text = (gchar*)bytes->get_data(size);
	
	// Copy the About text to the TextView buffer.

	Gtk::TextView* textview = nullptr;
	m_builder->get_widget("text_viewer", textview);
 		
	Glib::RefPtr<Gtk::TextBuffer> buffer(textview->get_buffer());
	buffer->set_text(text);

	// Compose title text.  The application version number is obtained from
	// the variable E_PACKAGE_VERSION define in the Makefile.am CPP_FLAGS,
	// so it will automatically follow changes made to the package version
	// in configure.ac.

	Glib::ustring title =
		Glib::ustring::compose(
_("\n<b>Enigma in the Wine Cellar</b>\n\n\
%1\n\n\
© 2005, 2021 Chris Sterne"),E_PACKAGE_VERSION);

	Gtk::Label* label = nullptr;
	m_builder->get_widget("title_text", label);
	label->set_label(title);

	// Connect to the button clicked signal, which will cause the AboutView
	// widget to emit a "Done" signal. 
	
	Gtk::Button* button = nullptr;
	m_builder->get_widget("ok_button", button);
	
	button->signal_clicked().connect(sigc::mem_fun(*this, &AboutView::do_done));
}

//-------------------------------------------------
// This method returns the view done signal server.
//-------------------------------------------------

Enigma::AboutView::type_signal_done Enigma::AboutView::signal_done()
{
	return m_signal_done;
}

//----------------------------------------------------
// This method is called the "Done" button is pressed.
//----------------------------------------------------

void Enigma::AboutView::do_done()
{	
	m_signal_done.emit();
}
