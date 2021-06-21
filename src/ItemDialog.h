/*------------------------------------*
 * Program: Enigma in the Wine Cellar *
 * Version: 2.0 for Linux OS          *
 * File:    MapDevices.h              *
 * Date:    March 3, 2016             *
 * Author:  Chris Sterne              *
 *                                    *
 * ItemDialog class header.           *
 *------------------------------------*/

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