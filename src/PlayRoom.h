//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 5.0 for Linux OS          *
//* File:    PlayRoom.h                *
//* Date:    October 15, 2016          *
//* Author:  Chris Sterne              *
//*                                    *
//* PlayRoom class header.             *
//*------------------------------------*
 
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