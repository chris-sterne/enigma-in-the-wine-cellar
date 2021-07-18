// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MapsView class header.  The MapsView class displays
// a list of available game maps with corresponding map description.
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
