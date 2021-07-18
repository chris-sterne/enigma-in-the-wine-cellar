// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the HelpView class implementation.  The HelpView class
// displays the game's instructions.
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
#include "HelpView.h"

//*-----------------------------*
//* Local declarations and data *
//*-----------------------------*

const char* KHelpText =_("\
ENIGMA IN THE WINE CELLAR\n\
\n\
While visiting your grandparent's country mansion nestled close to a small \
mountain, you discover a dark passage leading from an unused part of the wine \
cellar. Being an adventurous person, you grab your flashlight, compass and \
trusty Linux tablet, then proceed into the passage...\n\
\n\
OBJECTIVE\n\
Explore the rooms and passages while searching for special items.  The \
description of a game map will provide further details.  Beware of confusion \
traps that turn or move you unexpectedly.  If you fall into a dead-end pit, \
you will need to restart the game.\n\
\n\
PLAYERS\n\
The active player number is shown as (P)layer.  The 'Switch Player' option \
changes the active player.  Another player is represented by a smiling face \
in a location.\n\
\n\
ITEMS\n\
Move towards an item to get it and place it in your inventory.  Some items may \
be attached to the walls rather than resting on the floor.  The 'Inventory' \
option displays the inventory of the active player.  You can give an item to \
another player if there is one in your location.\n\
\n\
MECHANISMS\n\
Move towards a mechanism to activate it.  Some mechanisms require finding a \
key item, then selecting it from your inventory.  Once a mechanism key \
has been used, it will be automatically used the next time.\n\
\n\
GAME MAPS\n\
The 'Maps...' page shows a list of available game maps, including a \
description of the game map and the number of items remaining to be found.  \
To select a game map to play, either double-click or press Enter to select a \
highlighted list entry.\n\
\n\
SAVING THE GAME\n\
If permitted by the game map, the current state of the game can be saved to \
one reserved space for possible loading later.  This might be useful if you \
suspect a nearby dead-end pit.  Saving the game is not necessary before \
exiting the game or switching game maps, since the entire game map is \
automatically saved to a file at these times.\n\
\n\
GAME KEY CONTROL\n\
Several keys can be assigned to game actions in the 'Settings' page.  \
To assign a key to a game action, click on an action key list entry, then \
press a key to assign to that action.  By default, the keys are assigned as \
follows:\n\
\n\
Keypad 8: Move Forward\n\
Keypad 5: Move Backward\n\
Keypad 7: Move Left\n\
Keypad 9: Move Right\n\
Keypad 4: Turn left\n\
Keypad 6: Turn Right\n\
P key: Switch Player\n\
I key: Inventory\n\
\n\
GAME MOUSE CONTROL\n\
Several regions of the player's view page respond to mouse clicks, and are \
assigned game actions as follows:\n\
\n\
Above Center: Move Forward\n\
Below Center: Move Backward\n\
Lower Left: Move Left\n\
Lower Right: Move Right\n\
Left: Turn Left\n\
Right: Turn Right");

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CHelpView::CHelpView()
{
	// Prepare the TextView widget.
	
  iTextView = std::unique_ptr<Gtk::TextView>( new Gtk::TextView() );
  iTextView->set_wrap_mode( Gtk::WRAP_WORD );
  iTextView->set_editable(FALSE);
  iTextView->set_cursor_visible(FALSE);
  iTextView->set_hexpand(TRUE);
  iTextView->set_vexpand(TRUE);

	// Add help text to the TextView built-in buffer.
	
 	Glib::RefPtr<Gtk::TextBuffer> Buffer = iTextView->get_buffer();
	Buffer->set_text( KHelpText );

	// Add the TextView widget to a ScrolledWindow.
  // Scrollbar linking and control is added automatically.
	
	Gtk::ScrolledWindow* ScrolledWindow = Gtk::manage( new Gtk::ScrolledWindow() );
	ScrolledWindow->add( *iTextView );
    
	Gtk::Button* Button = Gtk::manage( new Gtk::Button( _("  Done  "), false ));
	Button->set_halign(Gtk::ALIGN_END);    
	Button->set_relief(Gtk::RELIEF_NORMAL);
	Button->set_border_width (10);
        
	attach( *ScrolledWindow, 0, 0, 1, 1);
	attach( *Button, 0, 1, 1, 1);

	// Connect to the button clicked signal, which will cause the HelpView widget
	// to emit a "Done" signal. 
	
	Button->signal_clicked().connect( sigc::mem_fun( *this, &CHelpView::On_Done ));
	return;
}

//*--------------------------------------------------*
//* This method returns the view done signal server. *
//*--------------------------------------------------*

CHelpView::type_signal_done CHelpView::signal_done()
{
  return m_signal_done;
}

//*-----------------------------------------------------*
//* This method is called the "Done" button is pressed. *
//*-----------------------------------------------------*

void CHelpView::On_Done()
{
	// Restore the focus on the TextView for the next time the HelpView is shown.
	// From observation, not doing this results in the Button acquiring the focus
	// next time (probably Gtk remembers that the Button was the last selected
	// widget).
	
	iTextView->grab_focus();

	// Emit the "view done" signal.
	
  m_signal_done.emit();
	return;
}
