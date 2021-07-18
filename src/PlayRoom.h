// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the PlayRoom class header.  The PlayRoom class allows a player
// to move through map rooms and interact with objects within the room. 
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
 
#ifndef __PLAYROOM_H__
#define __PLAYROOM_H__

#include <gtkmm.h>
#include "Map.h"
#include "ViewCone.h"
#include "Sounds.h"
#include "Transition.h"

class CPlayRoom : public sigc::trackable
{
  public:
    enum class Action
		{
      ENone = 0,
      EContinue,
      EMoveFront,
      EMoveBack,
      EMoveRight,
      EMoveLeft,
      ETurnRight,
      ETurnLeft,
      TOTAL
    };

    // Public methods.

    CPlayRoom();
    void SetSounds( gboolean aPlay );
    void SetMap( std::shared_ptr<CMap> aMap );
    void SetFullOrientation( gboolean aFullOrientation );
    void SetViewTransitions( gboolean aShow );
    void Explore( CPlayRoom::Action aAction );
    void Continue();
    void On_View_Done( gint64 aRenderTime );
    void SelectItem( std::list<CMapItem>::iterator aItem );
    void SwitchPlayer();

    // PlayRoom view signal accessor.
		
    typedef sigc::signal<void, guint8> type_signal_view;
    type_signal_view signal_view();

    // PlayRoom orientation signal accessor.
		
    typedef sigc::signal<void, const Glib::ustring&> type_signal_orientation;
    type_signal_orientation signal_orientation();
    
  private:
    // Private methods.

    void Do_Orientation();
    void Do_View( guint8 aSelect );
    void Do_Transition_Continue();
    void Do_Delay_Continue();
    void Do_Continue();
    void On_Continue();
    void Prepare();
    void UpdateSense( gboolean& aChange, CSounds::ID& aSoundID );
  
    // Private data.
    
    std::shared_ptr<CMap> iMap;                 // Shared game map.
    std::list<CMapPlayer>::iterator iPlayer;    // Active map player iterator.
    gboolean iFullOrientation;                  // Full orientation if TRUE.
    gboolean iViewTransitions;                  // View transitions if TRUE.    
    gboolean iFreeMoving;                       // TRUE if FreeMoving.
    guint8 iEdging;                             // Edging resurfacing stage.
    EnigmaWC::Direction iOldSurface;            // Edging old surface.
    std::unique_ptr<CSounds> iSounds;           // Room sound player.
    std::list<CPlayRoom::Action> iActionQueue;  // Action queue.
    CTransition iTransition;                    // Player transition path.
    
    // Lists of iterators to objects in the source room.

    std::list<std::list<CMapObject>::iterator> iObjects;
    std::list<std::list<CMapTeleporter>::iterator> iTeleporters;
    std::list<std::list<CMapItem>::iterator> iItems;
    std::list<std::list<CMapItem>::iterator> iFoundItems;
    std::list<std::list<CMapPlayer>::iterator> iPlayers;
    
    // Signal servers and slots.

    type_signal_orientation m_signal_orientation;   // Orientation signal.
    type_signal_view m_signal_view;                 // View signal.
    sigc::slot<void> m_slot_delay;                  // Visual delay. 
    sigc::slot<void> m_slot_idle;                   // Idle indication.
};

#endif // __PLAYROOM_H__
