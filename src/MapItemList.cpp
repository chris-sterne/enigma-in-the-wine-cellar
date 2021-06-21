//*-----------------------------------------------------------*
//* Program: Enigma in the Wine Cellar                        *
//* Version: 5.0 for Linux OS                                 *
//* File:    MapItemList.cpp                                  *
//* Date:    October 19, 2016                                 *
//* Author:  Chris Sterne                                     *
//*-----------------------------------------------------------*
//* This class manages a list of map items, which are objects *
//* a player can find.                                        *
//*-----------------------------------------------------------*

#include "MapItemList.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CMapItemList::CMapItemList()
{	
  iItemDialog = std::unique_ptr<CItemDialog>( new CItemDialog );
	return;
}

//*----------------------------------------------------------*
//* This method return the item list to the empty condition. *
//*----------------------------------------------------------*

void CMapItemList::Clear()
{
  // Clear the base class list.
	
  clear();
	
	// Hide the item dialog in case is it showing.
	
  iItemDialog->hide();	
	return;
}

//*---------------------------------------------------------*
//* This method sets the parent window for the item dialog. *
//*---------------------------------------------------------*
//* aParent: Reference to parent window.                    *
//*---------------------------------------------------------*

void CMapItemList::SetParentWindow( Gtk::Window& aParent )
{	
  iItemDialog->set_transient_for( aParent );
	return;
}

//*-----------------------------------------------------*
//* This method changes all items to their saved state. *
//*-----------------------------------------------------*

void CMapItemList::Load()
{
  std::list<CMapItem>::iterator Item;
	
  for ( Item = begin(); Item != end(); ++ Item )
  {
    (*Item).iActive   = (*Item).iActiveSaved;
    (*Item).iSelected = (*Item).iSelectedSaved;
    (*Item).iUsed     = (*Item).iUsedSaved;
    (*Item).iOwner    = (*Item).iOwnerSaved;
  }
	
  // Hide the item dialog in case is it showing.
	
  iItemDialog->hide();
	return;
}

//*-------------------------------------------*
//* This method saves the state of all items. *
//*-------------------------------------------*

void CMapItemList::Save()
{
  std::list<CMapItem>::iterator Item;
	
  for ( Item = begin(); Item != end(); ++ Item )
  {
    (*Item).iActiveSaved   = (*Item).iActive;
    (*Item).iSelectedSaved = (*Item).iSelected;
    (*Item).iUsedSaved     = (*Item).iUsed;
    (*Item).iOwnerSaved    = (*Item).iOwner;
  }
  
  return;
}

//*---------------------------------------------------------*
//* This method changes all items to their restart state. *
//*---------------------------------------------------------*

void CMapItemList::Restart()
{
  std::list<CMapItem>::iterator Item;
	
  for ( Item = begin(); Item != end(); ++ Item )
  {
    (*Item).iActive   = (*Item).iActiveRestart;
    (*Item).iSelected = (*Item).iSelectedRestart;
    (*Item).iUsed     = (*Item).iUsedRestart;
    (*Item).iOwner    = (*Item).iOwnerRestart;
  }
	
  // Hide the item dialog in case is it showing.
	
  iItemDialog->hide();
  return;
}

//*----------------------------------------------------------*
//* This method returns the number of items not yet found.   *
//*----------------------------------------------------------*
//* aRequired:  Reference to write remaining required items. *
//* aOptional:  Reference to write remaining optional items. *
//* aEasterEgg: Reference to write remaining Easter Eggs.    *
//* aSkull:     Reference to write remaining Skulls.         *
//*----------------------------------------------------------*

void CMapItemList::GetRemaining( guint& aRequired,
							                   guint& aOptional,
		        						         guint& aEasterEgg,
                                 guint& aSkull )
{
	// Clear all remaining counts.
	
	aRequired  = 0;
	aOptional  = 0;
	aEasterEgg = 0;
  aSkull     = 0;

	// Examine all items in the list.
	
	std::list<CMapItem>::iterator Item;

	for ( Item = begin(); Item != end(); ++ Item )
	{
		if ( (*Item).iPresence.GetState()
		  && (*Item).iActive )
		{
			// The item is present and has not yet been found.  Increment 
			// the appropriate remaining count.

			switch ( (*Item).iCategory )
			{
				case EnigmaWC::Category::ERequired:
					++ aRequired;
					break;

				case EnigmaWC::Category::EOptional:
					++ aOptional;
					break;

				case EnigmaWC::Category::EEasterEgg:
					++ aEasterEgg;
					break;

        case EnigmaWC::Category::ESkull:
					++ aSkull;
					break;
          
				default:
					break;
			}
		}
	}

	return;
}

//*---------------------------------------------------------------*
//* This method returns iterators to all items in a map location. *
//*---------------------------------------------------------------*
//* aLocation: Map location to be examined.                       *
//* aBuffer:   Buffer to receive MapItem iterators.               *
//*---------------------------------------------------------------*

void CMapItemList::Read( const CMapLocation& aLocation,
                         std::list<std::list<CMapItem>::iterator>& aBuffer )
{
  std::list<CMapItem>::iterator Item;

  for ( Item = begin(); Item != end(); ++ Item )
  {
    // If the item location is correct, add a copy of the item list iterator
    // to the provided buffer. 
    
    if ( (*Item).iLocation == aLocation )
      aBuffer.push_back( Item );
  }
  
  return;
}

//*---------------------------------------------------------------*
//* This method returns iterators to all items owned by a player. *
//*---------------------------------------------------------------*
//* aOwner:  Player number of owner.                              *
//* aBuffer: Buffer to receive MapItem iterators.                 *
//*---------------------------------------------------------------*

void CMapItemList::Read( guint aOwner,
                         std::list<std::list<CMapItem>::iterator>& aBuffer )
{
  std::list<CMapItem>::iterator Item;

  for ( Item = begin(); Item != end(); ++ Item )
  {
    // If the item has been found by the owner, add a copy of the item/
    // list iterator to the provided buffer. 
    
    if ( !(*Item).iActive
      && (*Item).iOwner == aOwner )
    {
      aBuffer.push_back( Item );
    }
  }
  
  return;
}

//*-------------------------------------------------*
//* This method returns the found state of an item. *
//*-------------------------------------------------*
//* aItem:  Iterator to MapItem.                    *
//* RETURN: TRUE if item has been found.            *
//*-------------------------------------------------*

gboolean CMapItemList::GetFound( std::list<CMapItem>::iterator& aItem )
{
  return ( !(*aItem).iActive );
}

//*--------------------------------------------------------------------*
//* This method is called when an item has been found.  An item dialog *
//* will be displayed with a description of the item.                  *
//*--------------------------------------------------------------------*
//* aItem: Iterator to MapItem just found.                             *
//* aOwner: Number of player who found item.                           *
//*--------------------------------------------------------------------*

void CMapItemList::SetFound( std::list<CMapItem>::iterator& aItem,
                             guint8 aOwner )
{
  // Return immediately if the item has already been found.
  
  if ( !(*aItem).iActive )
    return;

  // Mark the item has having been found by the provided owner.
  
  (*aItem).iActive = FALSE;
  (*aItem).iOwner  = aOwner;

  // Examine the item list to determine its new overall state. Begin
  // by assuming all required and optional items have been found.

  gboolean AllOptional = TRUE;
  gboolean AllRequired = TRUE;
  gboolean Another     = FALSE;

  std::list<CMapItem>::iterator Item;

	for ( Item = begin(); Item != end(); ++ Item )
	{ 
		if ( (*Item).iPresence.GetState() )
		{
		  if ( (*Item).iActive )
      {
        // The item in the list is present but has not yet been found,
        // so adjust the inventory state accordingly.

        if ( (*Item).iCategory == EnigmaWC::Category::ERequired )
          AllRequired = FALSE;
        else if ( (*Item).iCategory == EnigmaWC::Category::EOptional )
          AllOptional = FALSE;
      }

      // Indicate if the item just found is similar to one already owned.

      if (( (*aItem).iID == (*Item).iID )
        && !(*Item).iActive
        && ( (*Item).iOwner == aOwner )
        && ( aItem != Item ))
      {
        Another = TRUE;
      }
    }
  }
	
  // Choose an appropriate additional comment (e.g. "This is a secret bonus
  // item!") based on the item found.

  CItemDialog::ID CommentID = CItemDialog::ID::ENone;
	
  if ( (*aItem).iCategory == EnigmaWC::Category::ESkull )
    CommentID = CItemDialog::ID::EExpired;
  else if ( (*aItem).iCategory == EnigmaWC::Category::EEasterEgg )
    CommentID = CItemDialog::ID::ESecretBonus;
  else if ( AllOptional && AllRequired )
    CommentID = CItemDialog::ID::EAll;
  else if ( (*aItem).iCategory == EnigmaWC::Category::ERequired )
  {
    if ( AllRequired )
      CommentID = CItemDialog::ID::EAllRequired;
  }
  else if ( (*aItem).iCategory == EnigmaWC::Category::EOptional )
  {
    if ( AllOptional )
      CommentID = CItemDialog::ID::EAllBonus;
    else
      CommentID = CItemDialog::ID::EBonus;
  }

  // Display a dialog to describe the item just found.
		
  iItemDialog->Show( (*aItem).iID, Another, CommentID );
  return;
}

//*----------------------------------------------------------*
//* This method clears the Selected state of all items owned *
//* by a player.                                             *
//*----------------------------------------------------------*
//* aOwner: Player number of owner.                          *
//*----------------------------------------------------------*

void CMapItemList::ClearSelected( guint8 aOwner )
{
  std::list<CMapItem>::iterator Item;

  for ( Item = begin(); Item != end(); ++ Item )
  {
    if ( !(*Item).iActive
      && ((*Item).iOwner == aOwner ))
    {
      // Clear the Selected state of an item found by the player.
      
      (*Item).iSelected = FALSE;
    }
  }
  
  return;
}


//*----------------------------------------------------------------*
//* This method sets the Selected state of all similar items owned *
//* by a player.                                                   *
//*----------------------------------------------------------------*
//* aID:       ID of item.                                         *
//* aOwner:    Player number of owner.                             *
//* aSelected: Selection state to be set.                          *
//* RETURN:    FALSE if player does not own item.                  *
//*----------------------------------------------------------------*

gboolean CMapItemList::SetSelected( EnigmaWC::ID aID,
                                    guint8 aOwner,
                                    gboolean aSelected )
{
  gboolean Owned = FALSE;
  std::list<CMapItem>::iterator Item;

  for ( Item = begin(); Item != end(); ++ Item )
  {
    if ( ((*Item).iID == aID )
      && !(*Item).iActive
      && ((*Item).iOwner == aOwner ))
    { 
      // Set the Selected state of an item found by the player.

      (*Item).iSelected = aSelected;
      Owned = TRUE;
    }
  }
  
  return Owned;
}

//*----------------------------------------------------------------------*
//* This method returns the Selected state of an item owned by a player. *
//* Since SetSelected() makes all similar items selected, the selected   *
//* state of the first instance is sufficient.                           *
//*----------------------------------------------------------------------*
//* aID:    ID of item.                                                  *
//* aOwner: Player number of owner.                                      *
//* RETURN: FALSE if player does not own item.                           *
//*----------------------------------------------------------------------*

gboolean CMapItemList::GetSelected( EnigmaWC::ID iID,
                                    guint8 aOwner )
{
  std::list<CMapItem>::iterator Item;

  for ( Item = begin(); Item != end(); ++ Item )
  {
    if (((*Item).iID == iID )
      && !(*Item).iActive     
      && ((*Item).iOwner == aOwner ))
    {
      // Return the Selected state of the item found by the player.
    
      return (*Item).iSelected;
    }
  }

  // Return FALSE since the player does not own the item.
  
  return FALSE;
}

//*-----------------------------------------------------------*
//* This method uses a key item on a general object.          *
//*-----------------------------------------------------------*
//* aItem:  Iterator to general object to have its key used.  *
//* aOwner: Player number making use of key.                  *
//* RETURN: TRUE if Object's key was used, or none is needed. *
//*-----------------------------------------------------------*

gboolean CMapItemList::UseKey( std::list<CMapObject>::iterator& aObject,
                               guint8 aOwner )
{
  // Select ID of the key required for the object.
  
  EnigmaWC::ID KeyID;
  
  switch( (*aObject).iID )
  {
    case EnigmaWC::ID::ESphereLock:
      KeyID = EnigmaWC::ID::ESphereKey;
      break;
      
    case EnigmaWC::ID::ECubeLock:
      KeyID = EnigmaWC::ID::ECubeKey;
      break;
      
    case EnigmaWC::ID::EWaterLock:
      KeyID = EnigmaWC::ID::EWaterBottle;
      break;
    
    case EnigmaWC::ID::EAppleLock:
      KeyID = EnigmaWC::ID::EApple;
      break;
    
    case EnigmaWC::ID::EOrangeLock:
      KeyID = EnigmaWC::ID::EOrange;
      break;
    
    case EnigmaWC::ID::EWineLock:
      KeyID = EnigmaWC::ID::EWineBottle;
      break;
      
    case EnigmaWC::ID::ETree:
      KeyID = EnigmaWC::ID::EFertilizer;
      break;
      
    case EnigmaWC::ID::EWallEyes:
      KeyID = EnigmaWC::ID::EFish;
      break;
      
    default:
      // Return immediately since the object does not require a key.
      return TRUE;
  }

  // Set all similar key items owned by the player to the Used state if any
  // are present and selected (or have already been used). 

  gboolean Used = FALSE;
  std::list<CMapItem>::iterator Item;
  
  for ( Item = begin(); Item != end(); ++ Item )
  {
    if ( ((*Item).iID == KeyID )
      && !(*Item).iActive
      && ((*Item).iOwner == aOwner )
      && ((*Item).iSelected || (*Item).iUsed ))
    {
      Used = TRUE;
      (*Item).iUsed = Used;
    } 
  }
  
  return Used;
}