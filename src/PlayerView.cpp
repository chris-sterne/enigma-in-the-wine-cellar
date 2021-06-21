//*------------------------------------------------------*
//* Program: Enigma in the Wine Cellar                   *
//* Version: 5.0 for Linux OS                            *
//* File:    PlayerView.cpp                              *
//* Date:    October 21, 2016                            *
//* Author:  Chris Sterne                                *
//*------------------------------------------------------*
//* This class displays the player's view of a game map. *
//*------------------------------------------------------*

#include "EnigmaWC.h"
#include "PlayerView.h"

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CPlayerView::CPlayerView()
{ 
  // Initialize instance data.

  iFrontKey        = 0;
  iBackKey         = 0;
  iLeftKey         = 0;                          
  iRightKey        = 0;
  iTurnLeftKey     = 0;
  iTurnRightKey    = 0;
  iInventoryKey    = 0;
  iSwitchPlayerKey = 0;

  iViewCone.SetDepth( 4 );

  set_hexpand( TRUE );
  set_vexpand( TRUE );
  set_can_focus( TRUE );
  
  // Request events (keyboard, mouse, on_map_event).
  
  add_events( Gdk::KEY_PRESS_MASK
            | Gdk::BUTTON_PRESS_MASK
            | Gdk::STRUCTURE_MASK );
  
  // Set a signal handler for orientation signals from the PlayRoom.
			
  iPlayRoom.signal_orientation().connect( sigc::mem_fun( *this,
                                          &CPlayerView::On_Orientation ));

  // Set a signal handler for view signals from the PlayRoom.
  
  iPlayRoom.signal_view().connect( sigc::mem_fun( *this,
                                   &CPlayerView::On_View));    
  return;
}

//*-------------------------*
//* C++ default destructor. *
//*-------------------------*

CPlayerView::~CPlayerView()
{
  return;
}

//*-----------------------------------------------------*
//* Method to get preferred width of PlayerView widget. *
//*-----------------------------------------------------*

void CPlayerView::get_preferred_width_vfunc(int& minimum_width, int& natural_width) const
{
	minimum_width = 160;
	natural_width = minimum_width * 6;
	return;
}

//*------------------------------------------------------*
//* Method to get preferred height of PlayerView widget. *
//*------------------------------------------------------*

void CPlayerView::get_preferred_height_vfunc(int& minimum_height, int& natural_height) const
{
	minimum_height = 160;
	natural_height = minimum_height * 6;
	return;
}

//*------------------------------------------------------------------*
//* This method is called when the widget has been allocated a size. *
//*------------------------------------------------------------------*

void CPlayerView::on_size_allocate( Gtk::Allocation& allocation )
{
	// Pass the method to the base class.
	
	//CGLArea::on_size_allocate( allocation );
	GLArea::on_size_allocate( allocation );

	// Prepare the screen mapping for mouse input.

	iScreenInput.SetBounds( allocation );
	return;
}

//*---------------------------------------------------------------------*
//* This method is called when the widget is visible (has been mapped). *
//*---------------------------------------------------------------------*

bool CPlayerView::on_map_event( GdkEventAny* aEvent )
{
  // Have the drawing area grab keyboard focus.  Based on observations,
  // not doing so causes the first key press to be ignored (This only
  // occurs if the startup screen view was previously shown). 
  
  grab_focus();
  return FALSE;
}

//*-----------------------------------*
//* Method to draw PlayerView widget. *
//*-----------------------------------*

//gboolean CPlayerView::on_render()
bool CPlayerView::on_render(const Glib::RefPtr<Gdk::GLContext>& context)
{
  // Render the viewer's view.
  
  iViewCone.Render();
  
  // Inform the PlayRoom that the viewer's view has been rendered,
  // and provide the rendering time required.
  
  iPlayRoom.On_View_Done( iViewCone.GetRenderTime() );
  return TRUE;
}

//*----------------------------------------------*
//* Method to handle mouse button press events.  *
//*----------------------------------------------*
//* key_event: Pointer to GdkEventButton.        *
//* RETURN:    TRUE if button press was handled. *
//*----------------------------------------------*

bool CPlayerView::on_button_press_event( GdkEventButton* button_event )
{
	gboolean Handled = TRUE;
	
	if ( button_event->type == GDK_BUTTON_PRESS )
	{
		switch( iScreenInput.GetAction( button_event->x, button_event->y ))
		{
			case CScreenInput::EFront:
				iPlayRoom.Explore( CPlayRoom::Action::EMoveFront );
				break;

			case CScreenInput::ETurnLeft:
				iPlayRoom.Explore( CPlayRoom::Action::ETurnLeft );
				break;

			case CScreenInput::ETurnRight:
				iPlayRoom.Explore( CPlayRoom::Action::ETurnRight );
				break;

			case CScreenInput::EBack:
				iPlayRoom.Explore( CPlayRoom::Action::EMoveBack );
				break;

			case CScreenInput::ELeft:
				iPlayRoom.Explore( CPlayRoom::Action::EMoveLeft );
				break;

			case CScreenInput::ERight:
				iPlayRoom.Explore( CPlayRoom::Action::EMoveRight );
				break;

			default:
				Handled = FALSE;
				break;
		}
	}
	else
		Handled = FALSE;

	return Handled;
}

//*-------------------------------------------*
//* Method to handle key press events.        *
//*-------------------------------------------*
//* key_event: Pointer to GdkEventKey.        *
//* RETURN:    TRUE if key press was handled. *
//*-------------------------------------------*

bool CPlayerView::on_key_press_event( GdkEventKey* key_event )
{	
  // Send motion to the active player in the PlayRoom.

  gboolean Handled = TRUE;
	
  if ( key_event->type == GDK_KEY_PRESS )
  {
    int KeyValue = key_event->keyval;
	    		
    if ( KeyValue == iFrontKey )
      iPlayRoom.Explore( CPlayRoom::Action::EMoveFront );
    else if ( KeyValue == iBackKey )
      iPlayRoom.Explore( CPlayRoom::Action::EMoveBack );
    else if ( KeyValue == iLeftKey )
      iPlayRoom.Explore( CPlayRoom::Action::EMoveLeft );
    else if ( KeyValue == iRightKey )
      iPlayRoom.Explore( CPlayRoom::Action::EMoveRight );
    else if ( KeyValue == iTurnLeftKey )
      iPlayRoom.Explore( CPlayRoom::Action::ETurnLeft );
    else if ( KeyValue == iTurnRightKey )
      iPlayRoom.Explore( CPlayRoom::Action::ETurnRight );
    else if ( KeyValue == iInventoryKey )
      Do_Inventory();
    else if ( KeyValue == iSwitchPlayerKey )
      Do_Switch_Player();
    else
      Handled = FALSE;
  }
  else
    Handled = FALSE;

  return Handled;
}

//*--------------------------------------------------------------*
//* This method reads settings from a provided CSettings object. *
//*--------------------------------------------------------------*

void CPlayerView::SetSettings( CSettings& aSettings )
{	                     
  iFrontKey = aSettings.get_integer( CSettings::ACTION_KEY_GROUP,
                                     CSettings::MOVE_FORWARD );

  iBackKey = aSettings.get_integer( CSettings::ACTION_KEY_GROUP,
                                    CSettings::MOVE_BACKWARD );

  iLeftKey = aSettings.get_integer( CSettings::ACTION_KEY_GROUP,
                                    CSettings::MOVE_LEFT );

  iRightKey = aSettings.get_integer( CSettings::ACTION_KEY_GROUP,
                                     CSettings::MOVE_RIGHT );

  iTurnLeftKey = aSettings.get_integer( CSettings::ACTION_KEY_GROUP,
                                        CSettings::TURN_LEFT );

  iTurnRightKey = aSettings.get_integer( CSettings::ACTION_KEY_GROUP,
                                         CSettings::TURN_RIGHT );
                                         
  iInventoryKey = aSettings.get_integer( CSettings::ACTION_KEY_GROUP,
                                         CSettings::INVENTORY );

  iSwitchPlayerKey = aSettings.get_integer( CSettings::ACTION_KEY_GROUP,
                                         CSettings::SWITCH_PLAYER );

  iPlayRoom.SetViewTransitions( aSettings.get_boolean(
                                CSettings::MISCELLANEOUS_GROUP,
                                CSettings::VIEW_TRANSITIONS ));

  iPlayRoom.SetSounds( aSettings.get_boolean(
                       CSettings::MISCELLANEOUS_GROUP,
                       CSettings::GAME_SOUNDS ));

  iPlayRoom.SetFullOrientation( aSettings.get_boolean(
                                CSettings::MISCELLANEOUS_GROUP,
                                CSettings::FULL_ORIENTATION ));
	return;
}

//*---------------------------------------------*
//* This method sets the game map to be played. * 
//*---------------------------------------------*
//* aMap: Shared pointer to game map.           *
//*---------------------------------------------*

void CPlayerView::SetMap( std::shared_ptr<CMap> aMap )
{	
	// Pass the shared pointer to the PlayRoom and ViewCone.

  iViewCone.SetMap( aMap );
	iPlayRoom.SetMap( aMap );
	return;
}

//*-------------------------------------------------------------*
//* This method makes the active player select an item for use. *
//*-------------------------------------------------------------*
//* aItem: Iterator to item to be selected for use.             *
///*------------------------------------------------------------* 

void CPlayerView::SelectItem( std::list<CMapItem>::iterator aItem )
{
  // Pass the selected item to the PlayRoom.
  
  iPlayRoom.SelectItem( aItem );
  return;
}

//*----------------------------------------------------*
//* This method switches to the next available player. *
//*----------------------------------------------------*

void CPlayerView::SwitchPlayer()
{
  // Switch the PlayRoom player.
  
  iPlayRoom.SwitchPlayer();
  return;
}

//*----------------------------------------------*
//* This method returns the map location signal. *
//*----------------------------------------------*

CPlayerView::type_signal_orientation CPlayerView::signal_orientation()
{
  return m_signal_orientation;
}

//*-----------------------------------------------------------------*
//* This method is a signal handler for Orientation signal from the *
//* PlayRoom.  The orientation string is sent in a new signal.      *
//*-----------------------------------------------------------------*

void CPlayerView::On_Orientation( const Glib::ustring& aString )
{
  m_signal_orientation.emit( aString );
	return;
}

//*--------------------------------------------------------------*
//* This method is a signal handler for the View signal from the *
//* PlayRoom.  A rendering request is added to the queue.        *
//*--------------------------------------------------------------*
//* aSelect: View selction.                                      *
//*--------------------------------------------------------------*

void CPlayerView::On_View( guint8 aSelect )
{  
  iViewCone.Fill( aSelect );
  queue_render();
	return;
}

//*--------------------------------------------------*
//* This method returns the inventory signal server. *
//*--------------------------------------------------*

CPlayerView::type_signal_inventory CPlayerView::signal_inventory()
{
  return m_signal_inventory;
}

//*-------------------------------------------------------*
//* This method is called the "Inventory" key is pressed. *
//*-------------------------------------------------------*

void CPlayerView::Do_Inventory()
{	
  m_signal_inventory.emit();
  return;
}

//*--------------------------------------------------------*
//* This method emits a signal requesting a player switch. *
//*--------------------------------------------------------*

void CPlayerView::Do_Switch_Player()
{
  m_signal_switch_player.emit();
	return;
}

//*----------------------------------------------------*
//* This method returns the switch player signal slot. *
//*----------------------------------------------------*

CPlayerView::type_signal_switch_player CPlayerView::signal_switch_player()
{
  return m_signal_switch_player;
}
