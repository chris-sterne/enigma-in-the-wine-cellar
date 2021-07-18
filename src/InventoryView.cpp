// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the InventoryView class implementation.  The InventoryView
// class displays a player's inventory.
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
#include "EnigmaWC.h"
#include "InventoryView.h"
#include "Resources.h"

//*-----------------------------*
//* Local declarations and data *
//*-----------------------------*

struct ItemInfo
{
	EnigmaWC::ID iID;
	const char* iResourceName;
	const char* iSingularText;
	const char* iPluralText;
};

static ItemInfo ItemInfoArray[] =
{
  {
    EnigmaWC::ID::ENone,
    "None.png",
    _("unknown item"),
    _("unknown items")
  },
  {
    EnigmaWC::ID::EMushrooms,
    "Mushrooms.png",
    _("pair of mushrooms"),
    _("pairs of mushrooms")
  },
  {
  EnigmaWC::ID::ECorkScrew,
    "CorkScrew.png",
    _("bottle corkscrew"),
    _("bottle corkscrews")
  },
  {
    EnigmaWC::ID::EWineBottle,
    "WineBottle.png",
    _("bottle of wine"),
    _("bottles of wine")
  },
  {
    EnigmaWC::ID::EBread,
    "Bread.png",
    _("loaf of bread"),
    _("loaves of bread")
  },
  {
    EnigmaWC::ID::ECheese,
    "Cheese.png",
    _("block of cheese"),
    _("blocks of cheese")
  },
  {
    EnigmaWC::ID::EKnife,
    "Knife.png",
    _("stainless steel knife"),
    _("stainless steel knives")
  },
  {
    EnigmaWC::ID::EApple,
    "Apple.png",
    _("green apple"),
    _("green apples")
  },
  {
    EnigmaWC::ID::EWineGlass,
    "WineGlass.png",
    _("wine glass"),
    _("wine glasses")
  },
  {
    EnigmaWC::ID::EPlaceMat,
    "PlaceMat.png",
    _("checked placemat"),
    _("checked placemats")
  },
  {
    EnigmaWC::ID::EGrapes,
    "Grapes.png",
    _("bunch of grapes"),
    _("bunches of grapes")
  },
  {
    EnigmaWC::ID::ESkull,
    "Skull.png",
    _("human skull"),
    _("human skulls")
  },
  {
    EnigmaWC::ID::EEasterEgg,
    "EasterEgg.png",
    _("Easter Egg"),
    _("Easter Eggs")
  },
  {
    EnigmaWC::ID::EAirBalloon,
    "AirBalloon.png",
    _("air balloon"),
    _("air balloons")
  },
  {
    EnigmaWC::ID::EPlate,
    "Plate.png",
    _("porcelain plate"),
    _("porcelain plates")
  },
  {
    EnigmaWC::ID::EFork,
    "Fork.png",
    _("stainless steel fork"),
    _("stainless steel forks")
  },
  {
    EnigmaWC::ID::ECake,
    "Cake.png",
    _("chocolate cake"),
    _("chocolate cakes")
  },
  {
    EnigmaWC::ID::ECubeKey,
    "CubeKey.png",
    _("cube key"),
    _("cube keys")
  },
  {
    EnigmaWC::ID::ESphereKey,
    "SphereKey.png",
    _("sphere key"),
    _("sphere keys")
  },
  {
    EnigmaWC::ID::EEgg,
    "Egg.png",
    _("hard-boiled egg"),
    _("hard-boiled eggs")
  },
  {
		EnigmaWC::ID::EOrange,
		"Orange.png",
		_("Tangerine orange"),
		_("Tangerine oranges")
  },
	{
		EnigmaWC::ID::EFish,
		"Fish.png",
		_("fresh fish"),
		_("fresh fishes")
	},
	{
		EnigmaWC::ID::EWaterBottle,
		"WaterBottle.png",
		_("bottle of mineral water"),
		_("bottles of mineral water")
	},
	{
		EnigmaWC::ID::EFertilizer,
		"Fertilizer.png",
		_("box of plant fertilizer"),
		_("boxes of plant fertilizer")
	},
  {
    EnigmaWC::ID::EHydrogenBalloon,
    "HydrogenBalloon.png",
    _("hydrogen balloon"),
    _("hydrogen balloons")
  },	
  {
    EnigmaWC::ID::EInnerTube,
    "InnerTube.png",
    _("tire inner tube"),
    _("tire inner tubes")
  },
  {
    EnigmaWC::ID::EWeed,
    "Weed.png",
    _("tuft of water weeds"),
    _("tufts of water weeds")
  },
};

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CInventoryView::CInventoryView() : Gtk::Box( Gtk::ORIENTATION_VERTICAL )
{  
  // Initialize the thumbnail image storage.  For simplicity, all item images
  // are loaded at once, rather than waiting until they are needed. 

  int Width;
  int Height;
  guint Index;
  std::string ResourcePath;  
  Glib::RefPtr<Gdk::Pixbuf> Pixbuf;
  Glib::RefPtr<Gdk::Pixbuf> ScaledPixbuf;

  std::unique_ptr<Gtk::Image> Image =
    std::unique_ptr<Gtk::Image>( new Gtk::Image );
  
  Index = 0;
  
  do
  {
    // Build a full path to the 1x thumbnail image in the resources.
    
    ResourcePath.clear();
    ResourcePath = "/org/game/EnigmaWC/Bitmaps/";  
    ResourcePath.append( ItemInfoArray[ Index ].iResourceName );
    
    // Load the image from the resources, then create from it a scaled
    // Pixbuf that will be displayed as a thumbnail image.

    Image->set_from_resource( ResourcePath );
    Pixbuf = Image->get_pixbuf();

    Width  = Pixbuf->get_width() * 2;
    Height = Pixbuf->get_height() * 2; 
    
    ScaledPixbuf = Pixbuf->scale_simple( Width, Height, Gdk::INTERP_BILINEAR );
    iThumbnails.push_back( ScaledPixbuf );
  
    // Move to the next array entry, exiting the loop if the last entry
    // has been handled.
  
    ++ Index;
  }
  while ( Index < ( sizeof( ItemInfoArray ) / sizeof( struct ItemInfo )));
	
  // Prepare the display widgets.
	
  set_orientation( Gtk::ORIENTATION_VERTICAL );

  // Prepare a box to hold the radio buttons.
	
  Gtk::Box* Box = Gtk::manage( new Gtk::Box( Gtk::ORIENTATION_VERTICAL ));
  add( *Box );
  Box->set_border_width (10);
  Box->set_halign(Gtk::ALIGN_CENTER);

  // Keep a pointer to the RadioButtons for easy determination of which one
  // is active (Gtk::manage is not used on these child widgets).
	
  iRadioSelect = std::unique_ptr<Gtk::RadioButton>( new Gtk::RadioButton( _("Select Item") )); 
  Box->add( *iRadioSelect );
  iRadioGive = std::unique_ptr<Gtk::RadioButton>( new Gtk::RadioButton( _("Give Item") ));
  Box->add( *iRadioGive );
  iRadioSelect->set_sensitive( FALSE );
  iRadioGive->set_sensitive( FALSE );

  // Group the two RadioButtons together.
	
  Gtk::RadioButton::Group RadioGroup = iRadioSelect->get_group();
  iRadioGive->set_group( RadioGroup );

  // Prepare a ScrolledWindow and add a TreeView to it.
	
  Gtk::ScrolledWindow* ScrolledWindow = Gtk::manage( new Gtk::ScrolledWindow );
  add( *ScrolledWindow );
	
  iTreeView = std::unique_ptr<Gtk::TreeView>( new Gtk::TreeView );
  ScrolledWindow->add( *iTreeView );
  iTreeView->set_headers_visible( FALSE );
  iTreeView->set_hexpand(TRUE);
  iTreeView->set_vexpand(TRUE);
  iTreeView->set_activate_on_single_click( TRUE );
  iTreeView->set_can_focus(TRUE);
  iTreeView->set_enable_search( FALSE );

  Glib::RefPtr<Gtk::TreeSelection> TreeSelection = iTreeView->get_selection();
  TreeSelection->set_mode(Gtk::SELECTION_BROWSE);

  // Create a view "Done" button.

  Gtk::Button* Button = Gtk::manage( new Gtk::Button( _("  Done  "), false ));
  add( *Button );
  Button->set_halign(Gtk::ALIGN_END);    
  Button->set_relief(Gtk::RELIEF_NORMAL);
  Button->set_border_width (10);
	
  // Connect to the button clicked signal, which will cause the InventoryView widget
  // to emit a "Done" signal. 
		
  Button->signal_clicked().connect( sigc::mem_fun( *this,
                                    &CInventoryView::On_Done ));

  // Create the TreeView model.  A TreeModelFilter has a function added
  // for hiding rows with an item count of zero item.
	
  iListStore = Gtk::ListStore::create( iColumnRecord );
  iTreeModelFilter = Gtk::TreeModelFilter::create( iListStore ); 	
	
  iTreeModelFilter->set_visible_func( sigc::mem_fun( *this,
                    &CInventoryView::On_Visible_Function ));

  // Create TreeView view.  Each column cell has a function added for determining
  // how to display the column data.

  Gtk::TreeViewColumn* ThumbnailColumn =
    Gtk::manage( new Gtk::TreeViewColumn );

  Gtk::CellRendererPixbuf* ThumbnailCell =
    Gtk::manage( new Gtk::CellRendererPixbuf );

  ThumbnailColumn->pack_start( *ThumbnailCell, TRUE );
  ThumbnailColumn->set_cell_data_func(
                   *ThumbnailCell,
                   sigc::mem_fun( *this,
                   &CInventoryView::Thumbnail_Data_Function ));

  Gtk::TreeViewColumn* DescriptionColumn =
    Gtk::manage( new Gtk::TreeViewColumn );
  
  Gtk::CellRendererText* DescriptionCell =
    Gtk::manage( new Gtk::CellRendererText );
  
  DescriptionColumn->pack_start( *DescriptionCell, TRUE );
  DescriptionColumn->set_cell_data_func(
                     *DescriptionCell,
                     sigc::mem_fun( *this,
                     &CInventoryView::Description_Data_Function )); 

  iTreeView->append_column( *ThumbnailColumn );
  iTreeView->append_column( *DescriptionColumn );
	
  //Connect signal for single-click row selection.
	
  iTreeView->signal_row_activated().connect(sigc::mem_fun(*this,
             &CInventoryView::On_Row_Activated) );

  // Connect a signal handler that is called each time the TreeView becomes
  // visible (mapped).  From observations, this signal is emitted by the
  // TreeView only when it becomes visible for the first time, but the
  // ScrolledWindow that contains the TreeView emits the signal each time
  // it becomes visible.

  ScrolledWindow->add_events( Gdk::STRUCTURE_MASK );
  
  ScrolledWindow->signal_map_event().connect( sigc::mem_fun( *this,
                  &CInventoryView::On_TreeView_Map_Event ));
  return;
}

//*-----------------------------------*
//* This method sets the game map.    * 
//*-----------------------------------*
//* aMap: Shared pointer to game map. *
//*-----------------------------------*

void CInventoryView::SetMap( std::shared_ptr<CMap> aMap )
{	
  // Save game map information.

  iMap = aMap;
	return;
}

//*---------------------------------------------------------*
//* This function is used while sorting the MapItems by ID. *
//*---------------------------------------------------------*
//* aObject1: First MapItem.                                *
//* aObject2: Second MapItem.                               *
//* RETURN:   TRUE if First MapItem comes before second.    *
//*---------------------------------------------------------*

gboolean IDSort( std::list<CMapItem>::iterator& Item1,
                 std::list<CMapItem>::iterator& Item2 )
{
	int ID1 = (int)(*Item1).iID;
	int ID2 = (int)(*Item2).iID;

	return ( ID1 > ID2 );
}

//*-------------------------------------------------------------*
//* This method builds an inventory list for the active player. *
//*-------------------------------------------------------------*

void CInventoryView::Update()
{ 
  // Return immediately if no game map is loaded, since there will be
  // no active player with an inventory of items.

  if ( !iMap->GetLoaded() )
  {
    Do_Player( _("No game map") );

    // Disable both RadioButtons and ListView.

    iRadioSelect->set_sensitive( FALSE );
    iRadioGive->set_sensitive( FALSE );
    iTreeView->set_sensitive( FALSE );
  
    return;
  }

  // The displayed player numbers start from "1", since a player number
  // of zero is for internal use only.  Find out which player inventory
  // will be displayed,  and if another player is nearby that can be
  // given items.

  iActivePlayer = iMap->Players().GetActive();
  iOtherPlayer  = iActivePlayer;
  
  if ( iMap->Players().size() > 1 )
  {
    // The game map has more than one player.  Find out if another player
    // is near the active player.

    std::list<CMapPlayer>::iterator Player;
    
    for ( Player = iMap->Players().begin();
          Player != iMap->Players().end();
          ++ Player )
    {
      if  (( Player != iActivePlayer )
        && ( (*Player).iLocation == (*iActivePlayer).iLocation ))
      {
        // Another player has the same position as the active player.
        // Break out of the loop.  Any other players also present
        // are ignored.

	  		iOtherPlayer = Player;
	  		break;
      }
    }
  }

  if ( iActivePlayer == iOtherPlayer )
  {
  	// No other player is near the active player.
		
  	Glib::ustring String =
  		Glib::ustring::compose( _("Inventory P%1"), 
                              (*iActivePlayer).iNumber + 1 );
    Do_Player( String );

    // Enable the "Select Item" RadioButton and disable
    // the "Give Item" RadioButton.
		
    iRadioSelect->set_sensitive( TRUE );
    iRadioGive->set_sensitive( FALSE );
  }	
  else
  {
    // There is another player near the active player.
		
    Glib::ustring String = 
      Glib::ustring::compose( _("Inventory P%1, Near P%2"),
                              (*iActivePlayer).iNumber + 1,
                              (*iOtherPlayer).iNumber + 1 );
    Do_Player( String );

    // Enable the "Select Item" and "Give Item" RadioButtons.
		
    iRadioSelect->set_sensitive( TRUE );
    iRadioGive->set_sensitive( TRUE );
  }		

  // Make the "Select Item" RadioButton active, since this is most likely
  // the activity to perform with the player's inventory.
	
  iRadioSelect->set_active();

  // Clear the ListStore and sorted item list of old item entries.

  iTreeView->unset_model();
  iListStore->clear();  
  iSortedItems.clear();

  // Prepare a sorted list of items found by the active player.
  // All items with the same item ID will be grouped together in the list.
  // (e.g. CubeKey #1, CubeKey #2, BoiledEgg). 

  iMap->Items().Read( (*iActivePlayer).iNumber, iSortedItems );
  iSortedItems.sort( IDSort );

  std::list<std::list<CMapItem>::iterator>::iterator Item;
  std::list<std::list<CMapItem>::iterator>::iterator SimilarItem;
  EnigmaWC::ID ID = EnigmaWC::ID::ENone;
  Gtk::TreeModel::Row Row;
  Gtk::TreeModel::Row SelectRow;
  gboolean Select = FALSE;

  guint Index;
  
  Item = iSortedItems.begin();
  
  while ( Item != iSortedItems.end() )
  {
    if ( (*(*Item)).iID != ID )
    {
      // A different group of sorted item has been encountered, so create
      // a new ListStore row for this group (may contain only one member).

      ID = (*(*Item)).iID;
      Row = *( iListStore->append() );

      // Initialize the ColumnRecord index for accessing the description text
      // and thumbnail image arrays.  If the item ID is not recognized,
      // the entry will appear as "unknown item".
      
      for ( Index = (sizeof( ItemInfoArray ) / sizeof( struct ItemInfo )) - 1;
          ( Index > 0 ) && ( ItemInfoArray[ Index ].iID != ID );
            -- Index );
    
      // Initialize the ListStore ColumnRecord fields for the item. 

      Row[ iColumnRecord.iIndex ] = Index;
      Row[ iColumnRecord.iItem ]  = Item;
      Row[ iColumnRecord.iTotal ] = 1;
    }
    else
    {
      // The item is another member of the current group, so a ListStore row
      // already exists.  Increment the group member count.
      
      Row[ iColumnRecord.iTotal ] = Row[ iColumnRecord.iTotal ] + 1;
    }
 
    // If a member of the current item group has an active Selected state,
    // save its iterator for selecting later.
    
    if ( (*(*Item)).iSelected )
    {
      SelectRow = Row;
      Select    = TRUE;
    }
  
    // Move to the next item in the sorted item list.
  
    ++ Item;
  }
  
  // Add the filled model to the TreeView.
  
  iTreeView->set_model( iTreeModelFilter );
  iTreeView->set_search_column( -1 );

  // Highlight a TreeView row for a selected item group, and scroll
  // to the row if necessary.
 
  if ( Select )
  {
    Gtk::TreeIter FilterRow =
      iTreeModelFilter->convert_child_iter_to_iter( SelectRow );
				
    const Gtk::TreeModel::Path FilterPath( FilterRow );				
    iTreeView->set_cursor( FilterPath );
    iTreeView->scroll_to_row( FilterPath );    
  }

	return;
}

//*-----------------------------------------------------------------------*
//* This method is called to determine if a list row should be displayed. *
//* A row with zero items would exist if all its items were just given to *
//* another player.  Rather than display an item count of zero, this      *
//* list filter function allows hiding the row instead.                   *
//*-----------------------------------------------------------------------*

bool CInventoryView::On_Visible_Function( const Gtk::TreeModel::const_iterator& aIterator )
{
  bool Visible = FALSE;

  if ( aIterator )
  {
    // Do not display rows with a similar item count of zero.

    Gtk::TreeModel::Row Row = *aIterator;    
    Visible = ( Row[ iColumnRecord.iTotal ] != 0 );
  }

  return Visible;
}

//*---------------------------------------------------------------------------*
//* This method is called when the column thumbnail cells are to be rendered. *
//* An appropriate item thumbnail image is drawn.                             *
//*---------------------------------------------------------------------------*

void CInventoryView::Thumbnail_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                             				          const Gtk::TreeIter& aTreeIterator )
{
	// Exit if there is no iterator.

	if ( !aTreeIterator )
		return;

	Gtk::TreeModel::Row Row = *aTreeIterator;
	Gtk::CellRendererPixbuf* CellRenderer = (Gtk::CellRendererPixbuf*)( aCellRenderer );
	
	// Assign the thumbnail image to the renderer.

	CellRenderer->property_pixbuf() =
	  iThumbnails.at( Row[ iColumnRecord.iIndex ] );

	return;
}

//*-----------------------------------------------------------------------------*
//* This method is called when the column description cells are to be rendered. *
//* Data is read from the model and converted into an appropriate content       *
//* for display.                                                                *
//*-----------------------------------------------------------------------------*

void CInventoryView::Description_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                              				          const Gtk::TreeIter& aTreeIterator )
{	
  // Exit if there is no iterator.

  if ( !aTreeIterator )
    return;
	
  Gtk::TreeModel::Row Row = *aTreeIterator;
  Gtk::CellRendererText* CellRenderer = (Gtk::CellRendererText*)( aCellRenderer );
	
  // Assemble the item number with description text.
	
  Glib::ustring FullText;
  const char* Text;
  guint8 Index = Row[ iColumnRecord.iIndex ];
	
  if ( Row[ iColumnRecord.iTotal ] > 1 )
    Text = ItemInfoArray[ Index ].iPluralText;
  else
    Text = ItemInfoArray[ Index ].iSingularText;

  std::list<std::list<CMapItem>::iterator>::iterator Item =
    Row[ iColumnRecord.iItem ];

  if ( (*(*Item)).iSelected )
  {
    FullText =
    Glib::ustring::compose( "<span underline=\"double\">%1 %2</span>",
                            Row[ iColumnRecord.iTotal ],
                            Text );

    CellRenderer->property_markup() = FullText;
  }
  else
  {
    FullText =
    Glib::ustring::compose( "%1  %2",
                            Row[ iColumnRecord.iTotal ],
                            Text );

    CellRenderer->property_text() = FullText;
  }

  return;
}

//*----------------------------------------------*
//* This method is called when a row is selected. *
//*-----------------------------------------------*

void CInventoryView::On_Row_Activated( const Gtk::TreeModelFilter::Path& aPath,
                                       Gtk::TreeViewColumn* aColumn )
{
  // The provided path is for the TreeModelFilter, rather than for the child
  // TreeModel.  Convert the filtered path to the proper TreeModel path.

  const Gtk::TreeModel::Path& path =
    iTreeModelFilter->convert_path_to_child_path( aPath );

  Gtk::TreeModel::iterator Iterator = iListStore->get_iter( path );	
	
  if ( Iterator )
  {
    Gtk::TreeModel::Row Row = *Iterator;
    std::list<std::list<CMapItem>::iterator>::iterator Item;

    if ( iRadioSelect->get_active() )
    {
      // An item was selected for use.  Emit an iterator to this item
      // in a signal.
       
      Item = Row[ iColumnRecord.iItem ];
      Do_Selected( *Item );
    }
    else if ( iRadioGive->get_active() )
    {
      // An item was selected to be given to another player.

      if ( iActivePlayer != iOtherPlayer )
      {
        // Another player is near the active player.  Change the owner
        // of the selected item.

        Item = Row[ iColumnRecord.iItem ];
        (*(*Item)).iOwner = (*iOtherPlayer).iNumber;

        // If an InnerTube or HydrogenBalloon item was given to another
        // player, update the InnerTube and HydrogenBalloon selected state
        // in both players (the giving player may no longer have these
        // items for use, and the receiving player may now have one of these
        // items in use).
        
        if ( (*(*Item)).iID == EnigmaWC::ID::EInnerTube )
        {
          (*iActivePlayer).iInnerTube =
            iMap->Items().GetSelected( EnigmaWC::ID::EInnerTube,
                                       (*iActivePlayer).iNumber );
      
          (*iOtherPlayer).iInnerTube =
            iMap->Items().GetSelected( EnigmaWC::ID::EInnerTube,
                                       (*iOtherPlayer).iNumber );
        }
     
        if ( (*(*Item)).iID == EnigmaWC::ID::EHydrogenBalloon )
        {
          (*iActivePlayer).iHydrogenBalloon =
            iMap->Items().GetSelected( EnigmaWC::ID::EHydrogenBalloon,
                                       (*iActivePlayer).iNumber );
      
          (*iOtherPlayer).iHydrogenBalloon =
            iMap->Items().GetSelected( EnigmaWC::ID::EHydrogenBalloon,
                                       (*iOtherPlayer).iNumber );
        }

        // If there are more than one of this item, change the iterator
        // to the next one, and decrease the similar item total.

        if ( Row[ iColumnRecord.iTotal ] > 0 )
        {
          Row[ iColumnRecord.iTotal ] = Row[ iColumnRecord.iTotal ] - 1;
          Row[ iColumnRecord.iItem ]  = ++ Item;
        }
      }
    }
  }

  return;
}

//*---------------------------------------------------------*
//* This method returns the title bar update signal server. *
//*---------------------------------------------------------*

CInventoryView::type_signal_player CInventoryView::signal_player()
{
  return m_signal_player;
}

//*-----------------------------------------------------------*
//* This method emits a signal containing a string describing *
//* the inventory player information.                         *
//*-----------------------------------------------------------*

void CInventoryView::Do_Player( const Glib::ustring& aString )
{
  m_signal_player.emit( aString );
	return;
}

//*-------------------------------------------------------*
//* This method returns the item selection signal server. *
//*-------------------------------------------------------*

CInventoryView::type_signal_selected CInventoryView::signal_selected()
{
  return m_signal_selected;
}

//*-------------------------------------------------------------------*
//* This method emits a signal with an iterator to the selected item. *
//*-------------------------------------------------------------------*
//* aItem: Iterator to selected item.                                 *
//*-------------------------------------------------------------------*

void CInventoryView::Do_Selected( std::list<CMapItem>::iterator aItem )
{  
  m_signal_selected.emit( aItem );
	return;
}

//*--------------------------------------------------*
//* This method returns the view done signal server. *
//*--------------------------------------------------*

CInventoryView::type_signal_done CInventoryView::signal_done()
{
  return m_signal_done;
}


//*----------------------------------------------------------*
//* This method is called when the TreeView becomes visible. *
//*----------------------------------------------------------*

bool CInventoryView::On_TreeView_Map_Event( GdkEventAny* aEvent )
{
  // Have the TreeView acquire the input focus.

  iTreeView->grab_focus();
  return FALSE;
}

//*-----------------------------------------------------*
//* This method is called the "Done" button is pressed. *
//*-----------------------------------------------------*

void CInventoryView::On_Done()
{
	// Emit the "view done" signal.
	
  m_signal_done.emit();
	return;
}
