//*------------------------------------------------------*
//* Program: Enigma in the Wine Cellar                   *
//* Version: 5.0 for Linux OS                            *
//* File:    PlayerView.cpp                              *
//* Date:    Octoer 21, 2016                             *
//* Author:  Chris Sterne                                *
//*                                                      *
//* PlayerView class header.                             *
//*------------------------------------------------------*

#ifndef __PLAYERVIEW_H__
#define __PLAYERVIEW_H__

#include <gtkmm.h>
#include "GLArea.h"
#include "Map.h"
#include "ViewCone.h"
#include "PlayRoom.h"
#include "Settings.h"
#include "ScreenInput.h"

class CPlayerView : public Gtk::GLArea //public CGLArea
{
  public:
    // Public methods.

    CPlayerView();
    ~CPlayerView();
    void SetMap( std::shared_ptr<CMap> aMap );
    void SetSettings( CSettings& aSettings );
    void SelectItem( std::list<CMapItem>::iterator aItem );
    void SwitchPlayer();

    // Map location signal accessor.
	
    typedef sigc::signal<void, const Glib::ustring&> type_signal_orientation;
    type_signal_orientation signal_orientation();
	
    // Inventory signal accessor.

    typedef sigc::signal<void> type_signal_inventory;
    type_signal_inventory signal_inventory();

    // Switch player signal slot accessor.

    typedef sigc::signal<void> type_signal_switch_player;
    type_signal_switch_player signal_switch_player();

  protected:
    // Overridden base class methods.
	
	  bool on_map_event(GdkEventAny* aEvent );
    void on_size_allocate( Gtk::Allocation& allocation );
    bool on_key_press_event( GdkEventKey* key_event );
    virtual void get_preferred_width_vfunc(int& minimum_width, int& natural_width) const;
    virtual void get_preferred_height_vfunc(int& minimum_height, int& natural_height) const;
    bool on_button_press_event( GdkEventButton* button_event );
    void on_realize() override;
    //gboolean on_render();
    bool on_render(const Glib::RefPtr<Gdk::GLContext>& context) override;
	
  private:
    // Private methods.

    void Do_Inventory();
    void Do_Switch_Player();
    void On_View( guint8 aSelect );
    void On_Orientation( const Glib::ustring& aString );

    // Private data.

    CViewCone iViewCone;                    // Map viewing cone of player.
    CPlayRoom iPlayRoom;                    // Map room with player.
    CScreenInput iScreenInput;              // Screen input game actions.
    int iFrontKey;											    // Move Forward key value.
    int iBackKey;                           // Move Backward key value.
    int iLeftKey;                           // Move Left key value.                          
    int iRightKey;                          // Move Right key value.
    int iTurnLeftKey;                       // Turn Left key value.
    int iTurnRightKey;                      // Turn Right key value.
    int iInventoryKey;                      // Inventory key value.
    int iSwitchPlayerKey;                   // Switch player key value.

    // Signal servers and slots.

    type_signal_orientation m_signal_orientation;
    type_signal_inventory m_signal_inventory;
    type_signal_switch_player m_signal_switch_player;
};

#endif // __PLAYERVIEW_H__
