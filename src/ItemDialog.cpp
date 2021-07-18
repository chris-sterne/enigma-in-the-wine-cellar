// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the ItemDialog class implementation.  The ItemDialog class
// diplays a dialog with information about an item found. 
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
#include "ItemDialog.h"
#include "MapObject.h"
#include "Resources.h"

//*---------------------*
//* Local declarations. *
//*---------------------*

struct ItemInfo
{
	EnigmaWC::ID ItemID;
	const char* ResourceName;
	const char* FoundText;
	const char* AnotherText;
};

static ItemInfo ItemInfoArray[] =
{
  {
    EnigmaWC::ID::EMushrooms,
    "Mushrooms.png",
    _("You have found a pair of mushrooms."),
    _("You have found another pair of mushrooms.")
  },
	{
    EnigmaWC::ID::ECorkScrew,
    "CorkScrew.png",
    _("You have found a bottle corkscrew."),
    _("You have found another bottle corkscrew.")
  },
  {
    EnigmaWC::ID::EWineBottle,
    "WineBottle.png",
    _("You have found a bottle of wine."),
    _("You have found another bottle of wine.")
  },
  {
    EnigmaWC::ID::EBread,
    "Bread.png",
    _("You have found a loaf of bread."),
    _("You have found another loaf of bread.")
  },	
  {
    EnigmaWC::ID::ECheese,
    "Cheese.png",
    _("You have found a block of cheese."),
    _("You have found another block of cheese.")
  },
  {
    EnigmaWC::ID::EKnife,
    "Knife.png",
    _("You have found a stainless steel knife."),
    _("You have found another stainless steel knife.")
  },
  {
    EnigmaWC::ID::EApple,
    "Apple.png",
    _("You have found a green apple."),
    _("You have found another green apple.")
  },
  {
    EnigmaWC::ID::EWineGlass,
    "WineGlass.png",
    _("You have found a wine glass."),
    _("You have found another wine glass.")
  },
  {
    EnigmaWC::ID::EPlaceMat,
    "PlaceMat.png",
    _("You have found a checked placemat."),
    _("You have found another checked placemat.")
  },
  {
    EnigmaWC::ID::EGrapes,
    "Grapes.png",
    _("You have found a bunch of grapes."),
    _("You have found another bunch of grapes.")
  },
  {
    EnigmaWC::ID::ESkull,
    "Skull.png",
    _("You have found a human skull."),
    _("You have found another human skull.")
  },
  {
    EnigmaWC::ID::EEasterEgg,
    "EasterEgg.png",
    _("You have found an Easter Egg."),
    _("You have found another Easter Egg.")
  },
  {
    EnigmaWC::ID::EAirBalloon,
    "AirBalloon.png",
    _("You have found an air balloon."),
    _("You have found another air balloon.")
  },
  {
    EnigmaWC::ID::EPlate,
    "Plate.png",
    _("You have found a porcelain plate."),
    _("You have found another porcelain plate.")
  },
  {
    EnigmaWC::ID::EFork,
    "Fork.png",
    _("You have found a stainless steel fork."),
    _("You have found another stainless steel fork.")
  },
  {
    EnigmaWC::ID::ECake,
    "Cake.png",
    _("You have found a chocolate cake."),
    _("You have found another chocolate cake.")
  },
  {
    EnigmaWC::ID::ECubeKey,
    "CubeKey.png",
    _("You have found a cube key."),
    _("You have found another cube key.")
  },
  {
    EnigmaWC::ID::ESphereKey,
    "SphereKey.png",
    _("You have found a sphere key."),
    _("You have found another sphere key.")
  },
  {
    EnigmaWC::ID::EEgg,
    "Egg.png",
    _("You have found a hard-boiled egg."),
    _("You have found another hard-boiled egg.")
  },
	{
		EnigmaWC::ID::EOrange,
		"Orange.png",
		_("You have found a Tangerine orange."),
		_("You have found another Tangerine orange.")
	},
	{
		EnigmaWC::ID::EFish,
		"Fish.png",
		_("You have found a fresh fish."),
		_("You have found another fresh fish.")
	},
	{
		EnigmaWC::ID::EWaterBottle,
		"WaterBottle.png",
		_("You have found a bottle of mineral water."),
		_("You have found another bottle of mineral water.")
	},
	{
		EnigmaWC::ID::EFertilizer,
		"Fertilizer.png",
		_("You have found a box of plant fertilizer."),
		_("You have found another box of plant fertilizer.")
	},
	{
    EnigmaWC::ID::EInnerTube,
    "InnerTube.png",
    _("You have found a tire inner tube."),
    _("You have found another tire inner tube.")
  },
  {
    EnigmaWC::ID::EHydrogenBalloon,
    "HydrogenBalloon.png",
    _("You have found a hydrogen balloon."),
    _("You have found another hydrogen balloon.")
  },
  {
    EnigmaWC::ID::EWeed,
    "Weed.png",
    _("You have found a tuft of water weeds."),
    _("You have found another tuft of water weeds.")
  },	
  {
    EnigmaWC::ID::ENone,
    "",
    "",
    ""
  }
};

static const char* KBonusText       = _("This is a bonus item!");
static const char* KSecretBonusText = _("This is a secret bonus item!");
static const char* KExpiredText     = _("Unfortunately, today is your expiry date.");
static const char* KAllRequiredText = _("All required items have now been found!");
static const char* KAllBonusText    = _("All bonus items have now been found!");
static const char* KAllText         = _("All items have now been found!");

//*----------------------*
//* Default constructor. *
//*----------------------*

CItemDialog::CItemDialog()
{
	iFastFilter = TRUE;

	// Initialize the Found Item dialog.

	set_title( _("Found Item") );
	set_resizable( FALSE );
	add_button( _("Close"), Gtk::RESPONSE_CLOSE );
	set_modal( FALSE );

	// Keep pointers to the dialog widgets so they can be easily accessed
	// later to change their contents.
	
	iItemImage = std::unique_ptr<Gtk::Image>( new Gtk::Image() );
	iItemImage->set_padding( 5, 5 );

	iMessage  = std::unique_ptr<Gtk::Label>( new Gtk::Label() );

	iLayoutGrid = std::unique_ptr<Gtk::Grid>( new Gtk::Grid() );
	iLayoutGrid->set_column_homogeneous( FALSE );
	iLayoutGrid->attach( *iItemImage, 0, 0, 1, 1);
	iLayoutGrid->attach( *iMessage, 1, 0, 3, 1);

	// Add extra dialog widgets to the content area.
	
	Gtk::Box* Content = get_content_area();
  Content->add( *iLayoutGrid );
  Content->set_border_width( 5 );
  
  // Handle responses from the dialog.
  
  signal_response().connect( sigc::mem_fun( *this,
	                           &CItemDialog::On_Response ));
	return;
}

//*----------------------------------------------*
//* This method sets the image Fast Filter flag. *
//*----------------------------------------------*

void CItemDialog::SetFastFilter( gboolean aSet )
{
	iFastFilter = aSet;
	return;
}

//*---------------------------------------------*
//* This method displays the Found Item dialog. *
//*---------------------------------------------*

void CItemDialog::Show( EnigmaWC::ID aID,
                       gboolean aAnother,
                       CItemDialog::ID aCommentID )
{
	// Find the information array entry corresponding to the item's ID.

	guint8 Index;
	
	for ( Index = 0;
				(( ItemInfoArray[ Index ].ItemID != aID )
					&&
	   		( ItemInfoArray[ Index ].ItemID != EnigmaWC::ID::ENone ));
				Index ++
	  	);

	// Exit if the item ID was not found.
	
	if ( ItemInfoArray[ Index ].ItemID == EnigmaWC::ID::ENone )
		return;

	// Update the widget contents to reflect the found item.
	// Build the full message from the item found message, 
	// and include any additional comments.

	std::string FullMessage;
	
	if ( aAnother )
		FullMessage = ItemInfoArray[ Index ].AnotherText;
	else
		FullMessage = ItemInfoArray[ Index ].FoundText;

	const char *ExtraMessage;
		
  switch ( aCommentID )
  {
    case CItemDialog::ID::EBonus:
      ExtraMessage = KBonusText;
      break;
      
    case CItemDialog::ID::ESecretBonus:
      ExtraMessage = KSecretBonusText;
      break;
      
    case CItemDialog::ID::EExpired:
      ExtraMessage = KExpiredText;
      break;
      
    case CItemDialog::ID::EAllRequired:
      ExtraMessage = KAllRequiredText;
      break;
      
    case CItemDialog::ID::EAllBonus:
      ExtraMessage = KAllBonusText;
      break;
      
    case CItemDialog::ID::EAll:
      ExtraMessage = KAllText;
      break;

    default:
      ExtraMessage = NULL;
      break;
  }

  if ( ExtraMessage )
  {
    FullMessage.append( "\n" );
    FullMessage.append( ExtraMessage );
  }

  iMessage->set_label( FullMessage );

	// Build the full path to the item image in the resources.
	
	std::string ResourcePath = "/org/game/EnigmaWC/Bitmaps/";
	ResourcePath.append( ItemInfoArray[ Index ].ResourceName );

  // Load the item image from the resources.

  std::unique_ptr<Gtk::Image> Image =
    std::unique_ptr<Gtk::Image>( new Gtk::Image );
  
  Image->set_from_resource( ResourcePath );

  // The original item images were for a 160x160 Palm Pilot screen,
	// so they will be enlarged by 2x for use in the item dialog.
  
  Glib::RefPtr<Gdk::Pixbuf> Pixbuf = Image->get_pixbuf();
  
  int Width  = Pixbuf->get_width() * 2;
  int Height = Pixbuf->get_height() * 2; 

  Glib::RefPtr<Gdk::Pixbuf> ScaledPixbuf =
    Pixbuf->scale_simple( Width, Height, Gdk::INTERP_BILINEAR );
  
  iItemImage->set( ScaledPixbuf );

	// show_all() must be called to update all widgets.
	
	show_all();
  return;
}

//*------------------------------------------------------------------*
//* This method hides the Found Item dialog when the "Close" button" *
//* is pressed.                                                      *
//*------------------------------------------------------------------*

void CItemDialog::On_Response( int response_id )
{
  if ( response_id == Gtk::RESPONSE_CLOSE )
    hide();
}
