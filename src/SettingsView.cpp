// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the SettingsView class implementation.  The SettingsView
// class displays the game's settings.
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
#include "SettingsView.h"

//*---------------------*
//* Local declarations. *
//*---------------------*

struct ActionKeyInfo
{
	const char* iAction;
	const char* iSettingsKeyName;
};

static const ActionKeyInfo ActionKeys[] =		// Game action keys.
{
	{_("Move Forward"), CSettings::MOVE_FORWARD },
	{_("Move Backward"), CSettings::MOVE_BACKWARD },
	{_("Move Left"), CSettings::MOVE_LEFT },
	{_("Move Right"), CSettings::MOVE_RIGHT },
	{_("Turn Left"), CSettings::TURN_LEFT },
	{_("Turn Right"), CSettings::TURN_RIGHT },
	{_("Inventory"), CSettings::INVENTORY },
	{_("Switch Player"), CSettings::SWITCH_PLAYER },
	{ NULL, 0 }
};

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CSettingsView::CSettingsView()
{
	iChange = FALSE;

	// Create a label for game settings.

	Gtk::Label* Label = Gtk::manage( new Gtk::Label( _("Game Settings") ));
	attach( *Label, 0, 0, 1, 1 );
	Label->set_padding( 0, 3 );

	// Create a containter for miscellaneous boolean game setting controls.

	Gtk::Grid* Grid = Gtk::manage( new Gtk::Grid );
	attach( *Grid, 0, 1, 1, 1 );
	Grid->set_border_width( 10 );
	Grid->set_halign( Gtk::ALIGN_CENTER );
	Grid->set_column_homogeneous( FALSE );

	iStartup =
		std::unique_ptr<Gtk::CheckButton>( new Gtk::CheckButton(_("Startup screen") ));

	iTransitions =
		std::unique_ptr<Gtk::CheckButton>( new Gtk::CheckButton(_("View transitions") ));

	iOrientation =
		std::unique_ptr<Gtk::CheckButton>( new Gtk::CheckButton(_("Full orientation") ));

	iSounds =
		std::unique_ptr<Gtk::CheckButton>( new Gtk::CheckButton(_("Game sounds") ));

	// This forms a one column of option CheckButtons.
	
  //Grid->attach( *iStartup,       0, 0, 1, 1 );
  //Grid->attach( *iTransitions,   0, 1, 1, 1 );
  //Grid->attach( *iLocation,      0, 2, 1, 1 );
  //Grid->attach( *iSounds,        0, 3, 1, 1 );

	// This forms two columns of option CheckButtons.
	
	Grid->attach( *iStartup,       0, 0, 1, 1 );
  Grid->attach( *iTransitions,   0, 1, 1, 1 );
  Grid->attach( *iOrientation,   1, 0, 1, 1 );
  Grid->attach( *iSounds,        1, 1, 1, 1 );
	
	// Create a label for game action keys list.
	
	Label = Gtk::manage( new Gtk::Label( _("Action Keys") ));
	attach( *Label, 0, 2, 1, 1 );
	Label->set_padding( 0, 3 );
	
	// Create another ScrolledWindow and add a TreeView to it.  This TreeView
	// displays a list of keyboard keys that can be assigned game functions.

	iTreeView = std::unique_ptr<Gtk::TreeView>( new Gtk::TreeView );
	Gtk::ScrolledWindow* ScrolledWindow = Gtk::manage( new Gtk::ScrolledWindow );
	ScrolledWindow->add( *iTreeView );
	attach( *ScrolledWindow, 0, 3, 1, 1);

	iTreeView->set_headers_visible( FALSE );
  iTreeView->set_hexpand(TRUE);
  iTreeView->set_vexpand(TRUE);
	iTreeView->set_can_focus(TRUE);
	iTreeView->set_enable_search(TRUE);
	iTreeView->set_activate_on_single_click( TRUE );
  iTreeView->set_search_column( -1 );
  iTreeView->set_enable_search( FALSE );

	Glib::RefPtr<Gtk::TreeSelection> TreeSelection = iTreeView->get_selection();
	TreeSelection->set_mode(Gtk::SELECTION_BROWSE);

	//Connect signal for single-click row selection.
	
	iTreeView->signal_row_activated().connect(sigc::mem_fun(*this,
                                            &CSettingsView::On_Row_Activated) );

	// Create the TreeView model.
	
	iListStore = Gtk::ListStore::create( iColumnRecord );	
	iTreeView->set_model( iListStore );
	
	// Create TreeView view.  The column cell has a function added for determining
	// how to display the column data.
	
  Gtk::TreeViewColumn* ActionColumn = Gtk::manage( new Gtk::TreeViewColumn );
	iTreeView->append_column( *ActionColumn );
	Gtk::CellRendererText* ActionCell = Gtk::manage( new Gtk::CellRendererText );
  ActionColumn->pack_start( *ActionCell, TRUE );
	ActionColumn->set_cell_data_func( *ActionCell,
	                                   sigc::mem_fun( *this,
	                                   &CSettingsView::Action_Data_Function )); 

	Gtk::TreeViewColumn* CheckmarkColumn = Gtk::manage( new Gtk::TreeViewColumn );
	iTreeView->append_column( *CheckmarkColumn );
	Gtk::CellRendererToggle* CheckmarkCell = Gtk::manage( new Gtk::CellRendererToggle );
	CheckmarkColumn->pack_start( *CheckmarkCell, TRUE );
	CheckmarkColumn->set_cell_data_func( *CheckmarkCell,
	                  		              sigc::mem_fun( *this,
	                		                &CSettingsView::Checkmark_Data_Function ));

	CheckmarkCell->set_radio( FALSE );
	CheckmarkCell->set_activatable( TRUE );
	
	Gtk::TreeViewColumn* NameColumn = Gtk::manage( new Gtk::TreeViewColumn );
	iTreeView->append_column( *NameColumn );
	Gtk::CellRendererText* NameCell = Gtk::manage( new Gtk::CellRendererText );
  NameColumn->pack_start( *NameCell, TRUE );
	NameColumn->set_cell_data_func( *NameCell,
	                         		    sigc::mem_fun( *this,
	                                &CSettingsView::Name_Data_Function ));
	
	// Populate the ListStore with default game action key information.
	
	Gtk::TreeModel::Row Row;

	for ( guint8 Index = 0; ActionKeys[ Index ].iAction != NULL; Index ++ )
	{
		Row = *( iListStore->append() );
		Row[ iColumnRecord.iAction ]   = ActionKeys[ Index ].iAction;
		Row[ iColumnRecord.iKeyValue ] = 0;
		Row[ iColumnRecord.iChange ]   = FALSE;
	}

	// Create a "Done" button for closing the view.
	
	Gtk::Button* Button = Gtk::manage( new Gtk::Button( _("  Done  "), false ));
	Button->set_halign(Gtk::ALIGN_END);    
	Button->set_relief(Gtk::RELIEF_NORMAL);
	Button->set_border_width (10);
	attach( *Button, 0, 4, 1, 1);

	// Connect to the button clicked signal, which will cause the SettingsView widget
	// to emit a "Done" signal. 
	
	Button->signal_clicked().connect( sigc::mem_fun( *this, &CSettingsView::On_Done ));

	// Connect a key press event handler to the TreeView, but place it before
	// its default handler.  This allows capturing key press events that would
	// normally be used by the TreeView for entry cursor or activation.
	
	iTreeView->signal_key_press_event().connect( sigc::mem_fun( *this,
	                                             &CSettingsView::On_Key_Press ),
	                                       			 FALSE );
	return;
}

//*--------------------------------------------------------------*
//* This method reads settings from a provided CSettings object. *
//*--------------------------------------------------------------*

void CSettingsView::SetSettings( CSettings& aSettings )
{
  Gtk::TreeModel::iterator Iterator;
  int Index = 0;

  for ( Iterator = iListStore->children().begin();
      Iterator != iListStore->children().end();
      Iterator ++ )
  {
    (*Iterator)[ iColumnRecord.iKeyValue ] =
      aSettings.get_integer( CSettings::ACTION_KEY_GROUP,
                             ActionKeys[ Index ].iSettingsKeyName );
    Index ++;
  }

  iStartup->set_active( aSettings.get_boolean(
                        CSettings::MISCELLANEOUS_GROUP,
                        CSettings::STARTUP_SCREEN ));

  iTransitions->set_active( aSettings.get_boolean(
                            CSettings::MISCELLANEOUS_GROUP,
                            CSettings::VIEW_TRANSITIONS ));

  iOrientation->set_active( aSettings.get_boolean(
                            CSettings::MISCELLANEOUS_GROUP,
                            CSettings::FULL_ORIENTATION ));

  iSounds->set_active( aSettings.get_boolean(
                       CSettings::MISCELLANEOUS_GROUP,
                       CSettings::GAME_SOUNDS ));
	return;
}

//*-------------------------------------------------------------*
//* This method writes settings to a provided CSettings object. *
//*-------------------------------------------------------------*

void CSettingsView::GetSettings( CSettings& aSettings )
{
	Gtk::TreeModel::iterator Iterator;
	int Index = 0;
	
	for ( Iterator = iListStore->children().begin();
			Iterator != iListStore->children().end();
			Iterator ++ )
	{
		aSettings.set_integer( CSettings::ACTION_KEY_GROUP,
			                     ActionKeys[ Index ].iSettingsKeyName,
		                    	 (*Iterator)[ iColumnRecord.iKeyValue ] );
		Index ++;
	}

	aSettings.set_boolean( CSettings::MISCELLANEOUS_GROUP,
				            		 CSettings::STARTUP_SCREEN,
												 iStartup->get_active() );

	aSettings.set_boolean( CSettings::MISCELLANEOUS_GROUP,
				            		 CSettings::VIEW_TRANSITIONS,
												 iTransitions->get_active() );

	aSettings.set_boolean( CSettings::MISCELLANEOUS_GROUP,
				            		 CSettings::FULL_ORIENTATION,
												 iOrientation->get_active() );

	aSettings.set_boolean( CSettings::MISCELLANEOUS_GROUP,
				            		 CSettings::GAME_SOUNDS,
												 iSounds->get_active() );
	
	return;
}

//*------------------------------------------------------*
//* Method to handle key press events from the TreeView. *
//*------------------------------------------------------*
//* key_event: Pointer to GdkEventKey.                   *
//* RETURN:    TRUE if key press was handled.            *
//*------------------------------------------------------*

gboolean CSettingsView::On_Key_Press( GdkEventKey* key_event )
{
	// Exit immediately if no game action keys are to be changed or the event
	// is not due to a key press.  The event is allowed to propagate to other
	// handlers.

	gboolean Handled = FALSE;
	
	if ( !iChange || ( key_event->type != GDK_KEY_PRESS ))
		return Handled;

	// A game action key is to be changed.  Find the first game key flagged
	// for changing in the list.
		
	Gtk::TreeModel::iterator Iterator;

	for ( Iterator = iListStore->children().begin();
			Iterator != iListStore->children().end();
			Iterator ++ )
	{
		if ( (*Iterator)[ iColumnRecord.iChange ] )
		{
			// Change the keyvalue of the game action key.

			(*Iterator)[ iColumnRecord.iKeyValue ] = key_event->keyval;
			(*Iterator)[ iColumnRecord.iChange ]   = FALSE;

			// Indicate that the key event has been handled, then break out
			// of the loop.

			Handled = TRUE;
			break;
		}
	}

	// Leave the global "key change" flag set if any other game action keys
	// are still flagged for changing.
	
	iChange = FALSE;
	
	for ( Iterator = Iterator; Iterator != iListStore->children().end(); Iterator ++ )
		iChange = iChange || (*Iterator)[ iColumnRecord.iChange ];
	
	return Handled;
}

//*----------------------------------------------------------------*
//* This method is called when checkmark cells are to be rendered. *
//* The checkmark will be shown if the "Replace" data is FALSE.    *
//*----------------------------------------------------------------*

void CSettingsView::Checkmark_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                             				          const Gtk::TreeIter& aTreeIterator )
{
	// Exit if there is no iterator.

	if ( !aTreeIterator )
		return;

	Gtk::TreeModel::Row Row = *aTreeIterator;
	Gtk::CellRendererToggle* CellRenderer = (Gtk::CellRendererToggle*)( aCellRenderer );
	
	CellRenderer->property_active() = !Row[ iColumnRecord.iChange ];
	return;
}

//*-----------------------------------------------------------------*
//* This method is called when key action cells are to be rendered. *
//* Data is read from the model and converted into an appropriate   *
//* content for display.                                            *
//*-----------------------------------------------------------------*

void CSettingsView::Action_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                         				          const Gtk::TreeIter& aTreeIterator )
{	
	// Exit if there is no iterator.

	if ( !aTreeIterator )
		return;
	
	Gtk::TreeModel::Row Row = *aTreeIterator;
	Gtk::CellRendererText* CellRenderer = (Gtk::CellRendererText*)( aCellRenderer );

	// Set the key action text.
	
	CellRenderer->property_text() = Row[ iColumnRecord.iAction ];
	return;
}

//*---------------------------------------------------------------*
//* This method is called when key name cells are to be rendered. *
//* Data is read from the model and converted into an appropriate *
//* content for display.                                          *
//*---------------------------------------------------------------*

void CSettingsView::Name_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                        				          const Gtk::TreeIter& aTreeIterator )
{	
	// Exit if there is no iterator.

	if ( !aTreeIterator )
		return;
	
	Gtk::TreeModel::Row Row = *aTreeIterator;
	Gtk::CellRendererText* CellRenderer = (Gtk::CellRendererText*)( aCellRenderer );

	// Convert the game action key value to a readable name.
	
	gchar* Name = gdk_keyval_name( Row[ iColumnRecord.iKeyValue ] );

	if ( Name )
		CellRenderer->property_text() = Name;
	else
	  CellRenderer->property_text() = _("Unknown Key");
	   
	return;
}

//*--------------------------------------------------------------------*
//* This method is called when a row is activated (e.g. single-click). *
//*--------------------------------------------------------------------*

void CSettingsView::On_Row_Activated( const Gtk::TreeModel::Path& aPath,
                                       Gtk::TreeViewColumn* aColumn )
{
	Gtk::TreeModel::iterator Iterator = iListStore->get_iter( aPath );	
	
  if ( Iterator )
  {
		// Toggle the state of the "key change" flag in the entry.
		
		(*Iterator)[ iColumnRecord.iChange ] = !(*Iterator)[ iColumnRecord.iChange ];
		
		// Set a global "key change" flag if any keys are being changed.
		    
		iChange = FALSE;
		    
		for ( Iterator = iListStore->children().begin();
					Iterator != iListStore->children().end();
					Iterator ++ )
		{
			if ( (*Iterator)[ iColumnRecord.iChange ] )
				iChange = TRUE;
		}
	}

	return;
}

//*-----------------------------------------------------*
//* This method is called the "Done" button is pressed. *
//*-----------------------------------------------------*

void CSettingsView::On_Done()
{
	// In case the some action keys are still flagged to be changed,
	// clear this condition.  This allows the "Done" button to cancel
	// further changes, leaving the flagged keys unchanged.
	
	iChange = FALSE;
	
	Gtk::TreeModel::iterator Iterator;

	for ( Iterator = iListStore->children().begin();
					Iterator != iListStore->children().end();
					Iterator ++ )
	{
		(*Iterator)[ iColumnRecord.iChange ] = FALSE;
	}

	m_signal_done.emit();
	return;
}

//*--------------------------------------------------*
//* This method returns the view done signal server. *
//*--------------------------------------------------*

CSettingsView::type_signal_done CSettingsView::signal_done()
{
  return m_signal_done;
}
