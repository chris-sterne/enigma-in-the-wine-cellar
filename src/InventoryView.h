// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the InventoryView class header.  The InventoryView class
// displays a player's inventory.
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

#ifndef __INVENTORYVIEW_H__
#define __INVENTORYVIEW_H__

#include <gtkmm.h>
#include "Map.h"
#include "MapItem.h"

class CInventoryView : public Gtk::Box
{
  public:
    // Public methods.

    CInventoryView();
    void Do_Player( const Glib::ustring& aString );
    void Do_Selected( std::list<CMapItem>::iterator aItem );
    void SetMap( std::shared_ptr<CMap> aMap );
    void SetFastFilter( gboolean aSet );
    void Update();
    void On_Done();
    bool On_TreeView_Map_Event( GdkEventAny* aEvent );
    
    // Inventory title signal accessor.
    
    typedef sigc::signal<void, const Glib::ustring&> type_signal_player;
    type_signal_player signal_player();

    // Selected item signal accessor.
		
	  typedef sigc::signal<void, std::list<CMapItem>::iterator> type_signal_selected;
		type_signal_selected signal_selected();

		// View done signal accessor.
		
	  typedef sigc::signal<void> type_signal_done;
		type_signal_done signal_done();
		
	private:
		// Private methods.

		void On_Row_Activated( const Gtk::TreeModelFilter::Path& aPath,
                           Gtk::TreeViewColumn* aColumn );

		void Thumbnail_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                         				  const Gtk::TreeIter& aTreeIterator );

		void Description_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                  				          const Gtk::TreeIter& aTreeIterator );

		bool On_Visible_Function( const Gtk::TreeModel::const_iterator& iter );
		
		// Private classes.
		
		class CItemColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
        // Item information array index.

        Gtk::TreeModelColumn
          <guint> iIndex;

        // Iterator to first similar owned item in a list of iterators.

        Gtk::TreeModelColumn
          <std::list<std::list<CMapItem>::iterator>::iterator> iItem;

        // Total similar owned items.

        Gtk::TreeModelColumn
          <guint> iTotal;

				CItemColumns()
				{ 
					add( iIndex );
					add( iItem );
          add( iTotal );
				}
		};

		// Private data.

    std::shared_ptr<CMap> iMap;                            // Game map.
    CItemColumns iColumnRecord;                            // Model of list column data.
    std::unique_ptr<Gtk::TreeView> iTreeView;              // TreeView widget.
    Glib::RefPtr<Gtk::TreeModelFilter> iTreeModelFilter;   // Filter out zero entries.
    Glib::RefPtr<Gtk::ListStore> iListStore;               // Storage for data entries.
    std::unique_ptr<Gtk::RadioButton> iRadioSelect;        // "Select Item" RadioButton;
    std::unique_ptr<Gtk::RadioButton> iRadioGive;          // "Give Item" RadioButton;
    std::vector<Glib::RefPtr<Gdk::Pixbuf>> iThumbnails;    // Item thumbnail images.
    std::list<std::list<CMapItem>::iterator> iSortedItems; // Sorted item list.

    // Iterators to players.

    std::list<CMapPlayer>::iterator iActivePlayer;   // Active player.
    std::list<CMapPlayer>::iterator iOtherPlayer;    // Other nearby player.

    // Signal servers and slots.
    
    type_signal_player m_signal_player;     // Player information signal server.
    type_signal_selected m_signal_selected; // Selected item signal server.
    type_signal_done m_signal_done;         // View done signal server.
};

#endif // __INVENTORYVIEW_H__
