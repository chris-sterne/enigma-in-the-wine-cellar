//*---------------------------------------------------*
//* Program: Enigma in the Wine Cellar                *
//* Version: 1.0 for Linux OS                         *
//* File:    MapsView.cpp                             *
//* Date:    February 14, 2016                        *
//* Author:  Chris Sterne                             *
//*                                                   *
//* MapsView class.                                   *
//*---------------------------------------------------*
//* This class displays a list of available game maps *
//* with corresponding map description.               *
//*---------------------------------------------------*

#include <glibmm/i18n.h>
#include "Map.h"
#include "MapsView.h"

//*---------------------*
//* Local declarations. *
//*---------------------*

#define KMapDirectory "Maps"

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CMapsView::CMapsView()
{
  set_hexpand( TRUE );
  set_vexpand( TRUE );

  // Create a label for the game map list.

	Gtk::Label* Label = Gtk::manage( new Gtk::Label( _("Game Maps") ));
	attach( *Label, 0, 0, 1, 1 );
	Label->set_padding( 0, 3 );

	// Create a label for the game map description.

	Label = Gtk::manage( new Gtk::Label( _("Description") ));
	attach( *Label, 0, 2, 1, 1 );
	Label->set_padding( 0, 3 );
	
	// Create another ScrolledWindow and add a TreeView to it.  This TreeView
	// displays a list of available game map filenames.

	iTreeView = std::unique_ptr<Gtk::TreeView>( new Gtk::TreeView );
	Gtk::ScrolledWindow* ScrolledWindow = Gtk::manage( new Gtk::ScrolledWindow );
	ScrolledWindow->add( *iTreeView );
	attach( *ScrolledWindow, 0, 1, 1, 1);

	iTreeView->set_headers_visible( FALSE );
  iTreeView->set_hexpand(TRUE);
  iTreeView->set_vexpand(TRUE);

	// Create the TreeView model.
	
	iListStore = Gtk::ListStore::create( iColumnRecord );	
	iTreeView->set_model( iListStore );
	
	// Create TreeView view.  The column cell has a function added for determining
	// how to display the column data.

  Gtk::TreeViewColumn* FilenameColumn = Gtk::manage( new Gtk::TreeViewColumn );
  Gtk::CellRendererText* FilenameCell = Gtk::manage( new Gtk::CellRendererText );
  FilenameColumn->pack_start( *FilenameCell, TRUE );
	FilenameColumn->set_cell_data_func( *FilenameCell,
	                                     sigc::mem_fun( *this,
	                                     &CMapsView::Filename_Data_Function )); 

	iTreeView->append_column( *FilenameColumn );
	
	//Connect signal for double-click row activation.
	
	iTreeView->signal_row_activated().connect(sigc::mem_fun(*this,
                                            &CMapsView::On_Row_Activated) );

	Glib::RefPtr< Gtk::TreeSelection > TreeSelection = iTreeView->get_selection();

	// Add a signal handler for TreeView cursor changes.

	iTreeView->signal_cursor_changed().connect(sigc::mem_fun(*this,
                                            &CMapsView::On_Cursor_Changed ));

	// Create a ScrolledWindow and add a TextView widget to it.  This TextView
	// displays information about the game map.  Easy access to this TextView
	// for updating, a pointer to it will be saved (Gtk::manage() not used).
	
	iTextView = std::unique_ptr< Gtk::TextView >( new Gtk::TextView );
	ScrolledWindow = Gtk::manage( new Gtk::ScrolledWindow() );
	ScrolledWindow->add( *iTextView );
	attach( *ScrolledWindow, 0, 3, 1, 1);
	
	iTextView->set_wrap_mode( Gtk::WRAP_WORD );
  iTextView->set_editable(FALSE);
  iTextView->set_cursor_visible(FALSE);
  iTextView->set_hexpand(TRUE);
  iTextView->set_vexpand(TRUE);
	
	// Create a view "Done" button.
    
	Gtk::Button* Button = Gtk::manage( new Gtk::Button( _("  Done  "), false ));
	Button->set_halign(Gtk::ALIGN_END);    
	Button->set_relief(Gtk::RELIEF_NORMAL);
	Button->set_border_width (10);
	attach( *Button, 0, 4, 1, 1);
	
	// Connect to the button clicked signal, which will cause the MapsView widget
	// to emit a "Done" signal. 
	
	Button->signal_clicked().connect( sigc::mem_fun( *this, &CMapsView::On_Done ));
	return;
}

//*----------------------------------------------*
//* This method sets the selected game map name. *
//*----------------------------------------------*

void CMapsView::SetGameMap( const Glib::ustring& aName )
{
	iSelected = aName;
	Do_Filename();
	return;
}

//*----------------------------------------------*
//* This method gets the selected game map name. *
//*----------------------------------------------*

Glib::ustring& CMapsView::GetGameMap()
{
	return iSelected;
}

//*---------------------------------------------------*
//* This method builds a list of available game maps. *
//*---------------------------------------------------*

void CMapsView::ReadMaps()
{	
  // Clear the ListStore of old entries.

  iListStore->clear();

  // Create a game map to use for reading the game map descriptions.

  std::unique_ptr<CMap> Map = std::unique_ptr<CMap>( new CMap );
	
  // Create a list of all game map files.
	
  Glib::Dir Directory( KMapDirectory );
  std::list<std::string> Entries( Directory.begin(), Directory.end() );

  std::list<std::string>::iterator Iterator;
  Gtk::TreeModel::Row Row;
  std::string Filename;
	
  for ( Iterator = Entries.begin(); Iterator != Entries.end(); Iterator ++ )
  {
    // Assemble a game map filename.
		
    Filename = Glib::build_filename( KMapDirectory, *(Iterator) );

    // Load the game map to read its description.  The game map will first
    // clear any old game map it contains.

    Map->LoadFile( Filename );

    if ( Map->GetLoaded() )
    {
      // If a game map was successfully loaded, assemble a TreeView entry for it.

      Row = *( iListStore->append() );
      Row[ iColumnRecord.iFilename ] = *(Iterator);
			
      // Count items not yet found.
			
      guint Required;
      guint Optional;
      guint EasterEgg;
      guint Skull;

      Map->Items().GetRemaining( Required, Optional, EasterEgg, Skull );

      // Assemble a string containg a description of the game map and items remaining.

      Row[ iColumnRecord.iDescription ] = 
        Glib::ustring::compose(_("\
%1\n\n\
REMAINING ITEMS:\n\
Required = %2\n\
Bonus = %3\n\
Easter Eggs = %4\n\
Skulls = %5"),\
      Map->Description(),
      Required,
      Optional,
      EasterEgg,
      Skull );
			
      // Have the TreeView highlight the last selected item row,
      // scrolling to it if necessary.

      if ( *(Iterator) == iSelected )
      {
        const Gtk::TreeModel::Path Path( Row );
        iTreeView->set_cursor( Path );
        iTreeView->scroll_to_row( Path );
      }
    }
  }
	
  return;
}

//*--------------------------------------------------------------*
//* This signal handler is called when the cursor changes row.   *
//* The map description for game map under the cursor is updated *
//* in the TextView widget.                                      *
//*--------------------------------------------------------------*

void CMapsView::On_Cursor_Changed()
{
	// Get an iterator to the highlighted (selected) entry.

	Glib::RefPtr< Gtk::TreeSelection > Selection = iTreeView->get_selection(); 
	Gtk::TreeModel::iterator Iterator = Selection->get_selected();

	if ( Iterator )
  {
		Gtk::TreeModel::Row Row = *Iterator;

		// Add map description text to the TextView built-in buffer.
	
 		Glib::RefPtr<Gtk::TextBuffer> Buffer = iTextView->get_buffer();
		Glib::ustring Text = Glib::ustring( Row[ iColumnRecord.iDescription ] );
		Buffer->set_text( Text );
	}
	
	return;
}

//*-------------------------------------------------------------*
//* This method is called when a game map list row is activated *
//* (Double-clicked) to choose the game map for loading.        *
//*-------------------------------------------------------------*

void CMapsView::On_Row_Activated( const Gtk::TreeModel::Path& aPath,
                                   Gtk::TreeViewColumn* aColumn )
{
	Gtk::TreeModel::iterator Iterator = iListStore->get_iter( aPath );	
	
  if ( Iterator )
  {
		Gtk::TreeModel::Row Row = *Iterator;

		// Save the chosen game map filename.

		iSelected.clear();
		iSelected = Row[ iColumnRecord.iFilename ];

		// Send the selected game map filename as a string in a signal.

		Do_Filename();
	}

	return;
}

//*--------------------------------------------------------------------------*
//* This method is called when the column filename cells are to be rendered. *
//* Data is read from the model and converted into an appropriate content    *
//* for display.                                                             *
//*--------------------------------------------------------------------------*

void CMapsView::Filename_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                         			          const Gtk::TreeIter& aTreeIterator )
{	
	// Exit if there is no iterator.

	if ( !aTreeIterator )
		return;
	
	Gtk::TreeModel::Row Row = *aTreeIterator;
	Gtk::CellRendererText* CellRenderer = (Gtk::CellRendererText*)( aCellRenderer );

	// Set the filename string.

	Glib::ustring Text = Glib::filename_display_name( Row[ iColumnRecord.iFilename ] ); 
	CellRenderer->property_text() = Text;
	return;
}

//*-----------------------------------------------------*
//* This method is called the "Done" button is pressed. *
//*-----------------------------------------------------*

void CMapsView::On_Done()
{
	// Restore the focus on the TreeView for the next time the InventoryView is shown.
	// From observation, not doing this results in the Button acquiring the focus
	// next time (probably Gtk remembers that the Button was the last selected widget).
	
	iTreeView->grab_focus();

	// Emit the "view done" signal.
	
	m_signal_done.emit();
	return;
}

//*------------------------------------------------------------------------*
//* This method emits a signal with the Filename of the selected game map. *
//*------------------------------------------------------------------------*

void CMapsView::Do_Filename()
{
	// Send a game map Filename signal.  The filename will be a full path
	// (relative to the application directory) to the game map.
		
	std::string Filename = Glib::build_filename( KMapDirectory, iSelected );
	m_signal_filename.emit( Filename );
	return;
}

//*--------------------------------------------------*
//* This method returns the view done signal server. *
//*--------------------------------------------------*

CMapsView::type_signal_done CMapsView::signal_done()
{
  return m_signal_done;
}

//*-------------------------------------------------*
//* This method returns the filename signal server. *
//*-------------------------------------------------*

CMapsView::type_signal_filename CMapsView::signal_filename()
{
  return m_signal_filename;
}
