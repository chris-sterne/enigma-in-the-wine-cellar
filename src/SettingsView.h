/*--------------------------------------------*
 * Program: Enigma in the Wine Cellar         *
 * Version: 2.0 for Linux OS                  *
 * File:    SettingsView.cpp                  *
 * Date:    May 4, 2016                       *
 * Author:  Chris Sterne                      *
 *                                            *
 * SettingsView class header.                 *
 *--------------------------------------------*/

#ifndef __SETTINGSVIEW_H__
#define __SETTINGSVIEW_H__

#include <gtkmm.h>
#include "Settings.h"

class CSettingsView : public Gtk::Grid
{
  public:
    // Public methods.

    CSettingsView();
		void SetSettings( CSettings& aSettings );
		void GetSettings( CSettings& aSettings );

		// View done signal accessor.
		
	  typedef sigc::signal<void> type_signal_done;
		type_signal_done signal_done();
		
  private:
    // Private methods.

    void Checkmark_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                                  const Gtk::TreeIter& aTreeIterator );

    void Action_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                               const Gtk::TreeIter& aTreeIterator );

    void Name_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                             const Gtk::TreeIter& aTreeIterator );

    void On_Row_Activated( const Gtk::TreeModel::Path& aPath,
                           Gtk::TreeViewColumn* aColumn );

    gboolean On_Key_Press( GdkEventKey* key_event );
    void On_Done();

    // Private classes.
		
    class CItemColumns : public Gtk::TreeModel::ColumnRecord
    {
      public:
        Gtk::TreeModelColumn<Glib::ustring> iAction;        // Key game action name.
        Gtk::TreeModelColumn<int> iKeyValue;								// Key value.
        Gtk::TreeModelColumn<gboolean> iChange;             // TRUE to change key value.
				
      CItemColumns()
      { 
        add( iAction );
        add( iKeyValue );					
        add( iChange );
      }
    };	
		
		// Private data.

    CItemColumns iColumnRecord;                       // Model of list column data.
    std::unique_ptr< Gtk::CheckButton > iStartup;     // Startup screen button.
    std::unique_ptr< Gtk::CheckButton > iTransitions; // Transitions button.
    std::unique_ptr< Gtk::CheckButton > iOrientation; // Full orientation ratio button.
    std::unique_ptr< Gtk::CheckButton > iSounds;      // Play sounds ratio button.
    std::unique_ptr< Gtk::TextView > iTextView;       // TextView widget.
    std::unique_ptr<Gtk::TreeView> iTreeView;         // TreeView widget.
    Glib::RefPtr<Gtk::ListStore> iListStore;          // Storage for data entries.
    type_signal_done m_signal_done;                   // View done signal server.
    gboolean iChange;                                 // TRUE to change action key value.  
};

#endif // __SETTINGSVIEW_H__