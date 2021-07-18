// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the ItemDialog class header.  The ItemDialog class diplays
// a dialog with information about an item found. 
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

#ifndef ITEMDIALOG_H_
#define ITEMDIALOG_H_

#include <gtkmm.h>

class CItemDialog : public Gtk::Dialog
{	
  public:
    enum class ID
    {
      ENone = 0,
      EBonus,
      ESecretBonus,
      EExpired,
      EAllRequired,
      EAllBonus,
      EAll,
      TOTAL
    };
		
    // Public methods.

    CItemDialog();
    void SetFastFilter( gboolean aSet );
    
    void Show( EnigmaWC::ID aItemID,
              gboolean aAnother,
              CItemDialog::ID aCommentID );
    
    void On_Response( int response_id );

  private:
    // Private data.

    std::unique_ptr<Gtk::Grid> iLayoutGrid;   // Widget layout grid.
    std::unique_ptr<Gtk::Image> iItemImage;   // Item image widget.
    std::unique_ptr<Gtk::Label> iMessage;     // Item message widget.
    gboolean iFastFilter;                     // TRUE to use fast image filter.
};

#endif /* ITEMDIALOG_H_ */
