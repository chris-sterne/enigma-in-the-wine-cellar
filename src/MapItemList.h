// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MapItemList class header.  The MapItemList class
// manages a list of map items, which are objects a player can find. 
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
