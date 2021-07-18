// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the GameDialog class header.  The GameDialog class displays
// a dialog requesting confirmation for changes about to be made to the game
// map.  
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
 
#ifndef __GAMEDIALOG_H__
#define __GAMEDIALOG_H__

#include <gtkmm.h>

class CGameDialog : public Gtk::Dialog
{
	public:
		enum class ID
		{
			ELoad = 0,
			ESave,
			ERestart
		};
		
		// Public methods.

		CGameDialog( Gtk::Window& aParent );
		Gtk::ResponseType Run( CGameDialog::ID aID );

	private:
		// Private data.

		std::unique_ptr<Gtk::Label> iMessage;      // Item message widget.
};

#endif // __GAMEDIALOG_H__
