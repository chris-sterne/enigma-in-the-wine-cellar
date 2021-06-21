//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 5.0 for Linux OS          *
//* File:    MapItemList.h             *
//* Date:    October 15, 2016          *
//* Author:  Chris Sterne              *
//*                                    *
//* MapItemList class header.          *
//*------------------------------------*
 
#ifndef __MAPITEMLIST_H__
#define __MAPITEMLIST_H__

#include <gtkmm.h>
#include "MapItem.h"
#include "ItemDialog.h"

class CMapItemList : public std::list<CMapItem>
{
  public:
    // Public methods.
		
    CMapItemList();
    void Clear();
    void Load();
    void Save();
    void Restart();
    void SetParentWindow( Gtk::Window& aParent );
    void SetFound( std::list<CMapItem>::iterator& aItem, guint8 aOwner );
    void ClearSelected( guint8 aOwner );
    gboolean SetSelected( EnigmaWC::ID iID, guint8 aOwner, gboolean aSelected );
    gboolean GetFound( std::list<CMapItem>::iterator& aItem );
    gboolean GetSelected( EnigmaWC::ID iID, guint8 aOwner );
    gboolean UseKey( std::list<CMapObject>::iterator& aObject, guint8 aOwner );
    
    void Read( const CMapLocation& aLocation,
               std::list<std::list<CMapItem>::iterator>& aBuffer );

    void Read( guint aOwner,
               std::list<std::list<CMapItem>::iterator>& aBuffer );
    
    void GetRemaining( guint& aRequired,
                       guint& aOptional,
                       guint& aEasterEgg,
                       guint& aSkull );
                       
  private:
    // Private data.

    std::unique_ptr<CItemDialog> iItemDialog;   // Dialog describing found item.
};

#endif // __MAPITEMLIST_H__