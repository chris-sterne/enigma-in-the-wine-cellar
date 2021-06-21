/*--------------------------------------------*
 * Program: Enigma in the Wine Cellar         *
 * Version: 1.0 for Linux OS                  *
 * File:    MapsView.cpp                      *
 * Date:    November 5, 2015                  *
 * Author:  Chris Sterne                      *
 *                                            *
 * MapsView class header.                     *
 *--------------------------------------------*/

#ifndef __MAPSVIEW_H__
#define __MAPSVIEW_H__

#include <gtkmm.h>

class CMapsView : public Gtk::Grid
{
  public:
    // Public methods.

    CMapsView();
		void SetGameMap( const Glib::ustring& aName );
		Glib::ustring& GetGameMap();
		void ReadMaps();
		void Do_Filename();
		void On_Done();
		void On_Cursor_Changed();

		// View done signal accessor.
		
	  typedef sigc::signal<void> type_signal_done;
		type_signal_done signal_done();
		
		// Selected game map filename signal accessor.
		
	  typedef sigc::signal<void, const std::string&> type_signal_filename;
		type_signal_filename signal_filename();
		
	private:
		// Private methods.

		void On_Row_Activated( const Gtk::TreeModel::Path& aPath,
                            Gtk::TreeViewColumn* aColumn );
		
		void Filename_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
			        				          const Gtk::TreeIter& aTreeIterator );

		// Private classes.
		
		class CItemColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
				Gtk::TreeModelColumn<std::string> iFilename;    		// Game map filename.
				Gtk::TreeModelColumn<std::string> iDescription;   // Game map Description.

				CItemColumns()
				{ 
					add( iFilename );
					add( iDescription );
				}
		};	
		
		// Private data.

		CItemColumns iColumnRecord;                    // Model of list column data.
		std::unique_ptr< Gtk::TextView > iTextView;    // TextView widget.
		std::unique_ptr<Gtk::TreeView> iTreeView;      // TreeView widget.
		Glib::RefPtr<Gtk::ListStore> iListStore;       // Storage for data entries.
		type_signal_filename m_signal_filename;        // Map filename signal server.
		type_signal_done m_signal_done;                // View done signal server.
		Glib::ustring iSelected;                       // Last selected map filename.
};

#endif // __MAPSVIEW_H__