//*-------------------------------------*
//* Program: Enigma in the Wine Cellar  *
//* Version: 5.0 for Linux OS           *
//* File:    Application.h              *
//* Date:    October 7, 2016            *
//* Author:  Chris Sterne               *
//*-------------------------------------*
//* This class is the application.      *
//*-------------------------------------*

#include <glibmm/i18n.h>
#include "Application.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CApplication::CApplication() : Gtk::Application( "org.game.EnigmaWC" )
{
  Glib::set_application_name( _("Enigma in the Wine Cellar") );	
  return;
}

//*----------------------------------------------------------*
//* Private function to call CApplication On_Shutdown method *
//* when "shutdown" signal emitted.                          *
//*----------------------------------------------------------*

void shutdown( GtkApplication* app, CApplication* Application )
{
  Application->On_Shutdown();
  return;
}

//*----------------------------------------------*
//* Method called when application is activated. *
//*----------------------------------------------*

void CApplication::on_activate()
{
  // Connect to application "shutdown" signal.  A C++ interface could
  // not be found, so a C interface is used instead.

  g_signal_connect( this->gobj(), "shutdown", G_CALLBACK( shutdown ), this );
  
  // Create a MainWindow.  Since only a reference to this window is passed
	// to the base class, ownership is retained by the derived class,
	// so the derived class will handle the window's destruction.

	iWindow = std::unique_ptr<CWindow>( new CWindow );
	add_window( *iWindow );
	iWindow->signal_delete_event().connect( sigc::mem_fun( *this,
	                               &CApplication::On_Window_Delete ));

	// Create a container widget for the window MenuBar and Notebook.

	Gtk::Grid* Grid = Gtk::manage( new Gtk::Grid );
	iWindow->add( *Grid );	

  // Add a MenuBar.  All widgets of the MenuBar are marked to be destroyed
	// with the MenuBar, which will be done by the container in Window.

  Gtk::MenuBar* MenuBar = Gtk::manage( new Gtk::MenuBar() );
	Grid->attach( *MenuBar, 0, 0, 1, 1);
  
	// Add a Notebook widget.
	
	iViewBook = std::unique_ptr<Gtk::Notebook>( new Gtk::Notebook );
	Grid->attach( *iViewBook, 0, 1, 1, 1);
	iViewBook->set_show_tabs( FALSE );
	iViewBook->set_show_border( FALSE );

  // Assemble an AboutView page for the Notebook widget.
  
  iAboutView = std::unique_ptr< CAboutView >( new CAboutView );
	iAboutViewNumber = iViewBook->append_page( *iAboutView );

	iAboutView->signal_done().connect( sigc::mem_fun( *this,
	                                   &CApplication::On_View_Done ));

  // Assemble a CellarView page for the Notebook widget.

  Grid = Gtk::manage( new Gtk::Grid );
  iCellarViewNumber = iViewBook->append_page( *Grid );
  
  Gtk::Label* Label =
    Gtk::manage( new Gtk::Label( _("Entering the dark passage...") ));
  
  Grid->attach( *Label, 0, 0, 1, 1 );
  Label->set_padding( 0, 3 );
  iCellarView = std::unique_ptr< CCellarView >( new CCellarView );
  Grid->attach( *iCellarView, 0, 1, 1, 1 );

  iCellarView->signal_done().connect( sigc::mem_fun( *this,
                                      &CApplication::On_View_Done ));

	// Assemble a PlayerView page for the Notebook widget.  A label is also
	// added to receive information from the PlayerView widget.

	Grid = Gtk::manage( new Gtk::Grid );
	iPlayerViewNumber = iViewBook->append_page( *Grid );
	Label = Gtk::manage( new Gtk::Label );
	Label->set_justify( Gtk::JUSTIFY_CENTER );
	Grid->attach( *Label, 0, 0, 1, 1 );
	Label->set_padding( 0, 3 );
	iPlayerView = std::unique_ptr< CPlayerView >( new CPlayerView );
	Grid->attach( *iPlayerView, 0, 1, 1, 1 );
	
	iPlayerView->signal_orientation().connect( sigc::mem_fun( *Label,
	                                           &Gtk::Label::set_label ));
	                                        
	iPlayerView->signal_inventory().connect( sigc::mem_fun( *this,
	                                         &CApplication::On_Inventory ));

	iPlayerView->signal_switch_player().connect( sigc::mem_fun( *this,
	                                             &CApplication::On_Switch ));

  // Assemble an InventoryView page for the Notebook widget.

  Grid = Gtk::manage( new Gtk::Grid );
	iInventoryViewNumber = iViewBook->append_page( *Grid );
	Label = Gtk::manage( new Gtk::Label );
	Grid->attach( *Label, 0, 0, 1, 1 );
	Label->set_padding( 0, 3 );
	iInventoryView = std::unique_ptr< CInventoryView >( new CInventoryView );
	Grid->attach( *iInventoryView, 0, 1, 1, 1 );

	iInventoryView->signal_player().connect( sigc::mem_fun( *Label,
	                                         &Gtk::Label::set_label ));
	
	iInventoryView->signal_selected().connect( sigc::mem_fun( *this,
	                                  &CApplication::On_Item_Selected ));

	iInventoryView->signal_done().connect( sigc::mem_fun( *this,
	                                       &CApplication::On_View_Done ));
  
  // Assemble a MapsView page for the Notebook widget.

  iMapsView       = std::unique_ptr< CMapsView >( new CMapsView );
  iMapsViewNumber = iViewBook->append_page( *iMapsView );

  iMapsView->signal_filename().connect( sigc::mem_fun( *this,
                                        &CApplication::On_Map_Load ));
  
  iMapsView->signal_done().connect( sigc::mem_fun( *this,
                                    &CApplication::On_View_Done ));

  // Assemble a SettingsView page for the Notebook widget.

  iSettingsView = std::unique_ptr< CSettingsView >( new CSettingsView );
  iSettingsViewNumber = iViewBook->append_page( *iSettingsView );
	
  iSettingsView->signal_done().connect( sigc::mem_fun( *this,
                               &CApplication::On_Settings_Done ));

  // Assemble a HelpView page for the Notebook widget.
  
  iHelpView = std::unique_ptr< CHelpView >( new CHelpView );
  iHelpViewNumber = iViewBook->append_page( *iHelpView );

  iHelpView->signal_done().connect( sigc::mem_fun( *this,
                                    &CApplication::On_View_Done ));
  
  // Populate the MenuBar.
  
	Gtk::Menu* Menu;
  Gtk::MenuItem *MainItem, *SubItem;

  // Game menu.
	
	Menu = Gtk::manage( new Gtk::Menu() );
  MainItem = Gtk::manage( new Gtk::MenuItem( _("Game") ));
	MainItem->set_submenu( *Menu );  
  MenuBar->append( *MainItem );

  iLoadMenuItem =
    std::unique_ptr<Gtk::MenuItem>( new Gtk::MenuItem( _("Load") ));

  Menu->add( *iLoadMenuItem );

  iLoadMenuItem->signal_activate().connect( sigc::mem_fun( *this,
                                   &CApplication::On_Game_Load ));
  iSaveMenuItem =
    std::unique_ptr<Gtk::MenuItem>( new Gtk::MenuItem( _("Save") ));

  Menu->add( *iSaveMenuItem );

  iSaveMenuItem->signal_activate().connect( sigc::mem_fun( *this,
                                   &CApplication::On_Game_Save ));
  iRestartMenuItem =
    std::unique_ptr<Gtk::MenuItem>( new Gtk::MenuItem( _("Restart") ));

  Menu->add( *iRestartMenuItem );
  
  iRestartMenuItem->signal_activate().connect( sigc::mem_fun( *this,
                                      &CApplication::On_Game_Restart ));

  SubItem = Gtk::manage( new Gtk::SeparatorMenuItem() );
  Menu->add( *SubItem );
  SubItem = Gtk::manage( new Gtk::MenuItem( _("Maps...") ));
  Menu->add( *SubItem );

  SubItem->signal_activate().connect( sigc::mem_fun( *this,
                             &CApplication::On_Maps ));
  
  SubItem = Gtk::manage( new Gtk::SeparatorMenuItem() );
  Menu->add( *SubItem );
  SubItem = Gtk::manage( new Gtk::MenuItem( _("Quit") ));
  Menu->add( *SubItem );
  
  SubItem->signal_activate().connect( sigc::mem_fun( *this,
                             &CApplication::quit ));

  // Player menu.
	
  Menu = Gtk::manage( new Gtk::Menu() );
  MainItem = Gtk::manage( new Gtk::MenuItem( _("Player") ));
  MainItem->set_submenu( *Menu );  
  MenuBar->append( *MainItem );	
  SubItem = Gtk::manage( new Gtk::MenuItem( _("Inventory...") ));
  Menu->add( *SubItem );
  
  SubItem->signal_activate().connect( sigc::mem_fun( *this,
                             &CApplication::On_Inventory ));  
  
  iSwitchMenuItem =
    std::unique_ptr<Gtk::MenuItem>( new Gtk::MenuItem( _("Switch") ));
  
  Menu->add( *iSwitchMenuItem );

	iSwitchMenuItem->signal_activate().connect( sigc::mem_fun( *this,
                                       &CApplication::On_Switch ));

  // Options menu.
	
  Menu = Gtk::manage( new Gtk::Menu() );
  MainItem = Gtk::manage( new Gtk::MenuItem( _("Options") ));
  MainItem->set_submenu( *Menu );  
  MenuBar->append( *MainItem );
  SubItem = Gtk::manage( new Gtk::MenuItem( _("Settings") ));
  Menu->add( *SubItem );
  
  SubItem->signal_activate().connect( sigc::mem_fun( *this,
                             &CApplication::On_Settings ));

  // Help menu.
	
  Menu = Gtk::manage( new Gtk::Menu() );
  MainItem = Gtk::manage( new Gtk::MenuItem( _("Help") ));
  MainItem->set_submenu( *Menu );
  MenuBar->append( *MainItem );	
  SubItem = Gtk::manage( new Gtk::MenuItem( _("Instructions") ));
  
  SubItem->signal_activate().connect( sigc::mem_fun( *this,
                             &CApplication::On_Help ));

  Menu->add( *SubItem );
  SubItem = Gtk::manage( new Gtk::MenuItem( _("About") ));
  Menu->add( *SubItem );
  
  SubItem->signal_activate().connect( sigc::mem_fun( *this,
                             &CApplication::On_About ));

  // Create a Game Load/Save/Restart Dialog.

  iGameDialog = std::unique_ptr<CGameDialog>( new CGameDialog( *iWindow ));

  // Create a shared game map, and set the map in all viewers.  This game map
  // is initially empty (unloaded).  The main application window is passed
  // to the Items list for use by the item dialog.

  iMap = std::shared_ptr<CMap>( new CMap ); 
  iMap->Items().SetParentWindow( *iWindow );

  iPlayerView->SetMap( iMap );
  iInventoryView->SetMap( iMap );

  // Load the application settings.
	
	iSettings = std::unique_ptr< CSettings >( new CSettings );
	iSettings->Load();

  // Pass settings to various objects.

	iSettingsView->SetSettings( *iSettings );
	iPlayerView->SetSettings( *iSettings );
	
  iStartupScreen = iSettings->get_boolean( CSettings::MISCELLANEOUS_GROUP,
                                           CSettings::STARTUP_SCREEN );
  	
  // Open the application window.

  iWindow->show_all();

  // Send the last selected game map name to the MapsView, causing it to emit
  // the filename as a signal with the full path filename.  This signal
  // is handled by the On_Map_Load() method.

  Glib::ustring Filename = iSettings->get_string( CSettings::GAME_MAP_GROUP,
                                                  CSettings::GAME_MAP );

  iMapsView->SetGameMap( Filename ); 
  return;
}

//*--------------------------------------------------*
//* Method called when application is shutting down. *
//*--------------------------------------------------*

void CApplication::On_Shutdown()
{
  // Save current game map and settings.

  iMap->SaveFile();
  iSettings->Save();

  return;
}

//*-----------------------------------------------------------------*
//* This method is the signal handler for the "Settings" menu item. * 
//*-----------------------------------------------------------------*

void CApplication::On_Settings()
{ 
  // Make the SettingsView view visible.

  iViewBook->set_current_page( iSettingsViewNumber );
  return;
}

//*------------------------------------------------------*
//* This method is called when the SettingsView is done. *
//*------------------------------------------------------*

void CApplication::On_Settings_Done()
{ 
  // Update any changes made to the settings by the SettingsView.

  iSettingsView->GetSettings( *iSettings );

  // Pass new settings to various objects.

  iPlayerView->SetSettings( *iSettings );

  iStartupScreen = iSettings->get_boolean( CSettings::MISCELLANEOUS_GROUP,
                                           CSettings::STARTUP_SCREEN );

  // Make the PlayerView view visible.

  iViewBook->set_current_page( iPlayerViewNumber );
	return;
}


//*------------------------------------------------------------------*
//* This method is the signal handler for the "Inventory" menu item. * 
//*------------------------------------------------------------------*

void CApplication::On_Inventory()
{ 
	// Prepare the InventoryView.

	iInventoryView->Update();

	// Make the InventoryView view visible.

	iViewBook->set_current_page( iInventoryViewNumber );
	return;
}

//*-----------------------------------------------------------------*
//* This method is the signal handler for the item selection signal *
//* from the InventoryView object.                                  * 
//*-----------------------------------------------------------------*

void CApplication::On_Item_Selected( std::list<CMapItem>::iterator aItem )
{ 
	// Make the PlayerView view visible.

	iViewBook->set_current_page( iPlayerViewNumber );
	
	// Pass the selected item to the PlayerView.

	iPlayerView->SelectItem( aItem );
	return;
}

//*---------------------------------------------------------------------*
//* This method is the signal handler for the player "Switch" menu item *
//*---------------------------------------------------------------------*

void CApplication::On_Switch()
{ 
  iPlayerView->SwitchPlayer();

  // If the InventoryView view is showing, update the item list for the
  // new active player.

  if ( iViewBook->get_current_page() == iInventoryViewNumber )
    iInventoryView->Update();

	return;
}

//*---------------------------------------------------------------------*
//* This method is the signal handler for the game "Maps..." menu item. * 
//*---------------------------------------------------------------------*

void CApplication::On_Maps()
{ 
	// Save the current game map to a file, which ensures that the remaining
	// item count shown in its map description correctly reflects the current
	// game.

	iMap->SaveFile();

	// Prepare the MapsView.

	iMapsView->ReadMaps();

	// Make the MapsView view visible.

	iViewBook->set_current_page( iMapsViewNumber );
	return;
}

//*--------------------------------------------------------------*
//* This method is the signal handler for the game map selection *
//* signal from the MapsView object.                             * 
//*--------------------------------------------------------------*

void CApplication::On_Map_Load( const std::string aFilename )
{  
  // Save current game map to a file.

  iMap->SaveFile();

  // Load new game map from a file.

  iMap->LoadFile( aFilename );

  // Set the new game map in all viewers. 

  iPlayerView->SetMap( iMap );
  iInventoryView->SetMap( iMap );

  // Adjust the sensitivity of some menu items based on the loaded map.

  gboolean CanLoad;
  gboolean CanSave;
  gboolean CanRestart;
  gboolean CanSwitch;
	
  if ( iMap->GetLoaded() )
  {
    CanSave    = iMap->GetSavable();
    CanLoad    = CanSave;
    CanRestart = TRUE;
    CanSwitch  = ( iMap->Players().size() > 1 );
  }
  else
  {
    CanSave    = FALSE;
    CanLoad    = FALSE;
    CanRestart = FALSE;
    CanSwitch  = FALSE;
  }
	
  iLoadMenuItem->set_sensitive( CanLoad );
  iSaveMenuItem->set_sensitive( CanSave );
  iRestartMenuItem->set_sensitive( CanRestart );
  iSwitchMenuItem->set_sensitive( CanSwitch );

  // Update the Settings object with the selected game map name.

	iSettings->set_string( CSettings::GAME_MAP_GROUP,
				            		 CSettings::GAME_MAP,
												 iMapsView->GetGameMap() );

	// Make PlayerView view visible, or show the startup screen first.

  if ( iStartupScreen )
    iViewBook->set_current_page( iCellarViewNumber );
  else
		iViewBook->set_current_page( iPlayerViewNumber );
  
  return;
}

//*------------------------------------------------------------------*
//* This method is the signal handler for the game "Load" menu item. * 
//*------------------------------------------------------------------*

void CApplication::On_Game_Load()
{ 	
  Gtk::ResponseType Response = iGameDialog->Run( CGameDialog::ID::ELoad );

  if ( Response == Gtk::RESPONSE_OK )
  {
    iMap->Load();
    
    // Notify all viewers about a change in the game map.

    iPlayerView->SetMap( iMap );
    iInventoryView->SetMap( iMap );

    // Show the PlayerView in case another view is showing.

    if ( iViewBook->get_current_page() != iPlayerViewNumber )
      iViewBook->set_current_page( iPlayerViewNumber );
  }
	
	return;
}

//*------------------------------------------------------------------*
//* This method is the signal handler for the game "Save" menu item. * 
//*------------------------------------------------------------------*

void CApplication::On_Game_Save()
{ 
  Gtk::ResponseType Response = iGameDialog->Run( CGameDialog::ID::ESave );

  if ( Response == Gtk::RESPONSE_OK )
    iMap->Save();

	return;
}

//*---------------------------------------------------------------------*
//* This method is the signal handler for the game "Restart" menu item. * 
//*---------------------------------------------------------------------*

void CApplication::On_Game_Restart()
{ 
  Gtk::ResponseType Response = iGameDialog->Run( CGameDialog::ID::ERestart );

  if ( Response == Gtk::RESPONSE_OK )
  {
    iMap->Restart();
    
    // Notify all viewers about a change in the game map.

    iPlayerView->SetMap( iMap );
    iInventoryView->SetMap( iMap );

	  // Make PlayerView view visible, or show the startup screen first.

    if ( iStartupScreen )
      iViewBook->set_current_page( iCellarViewNumber );
    else
		  iViewBook->set_current_page( iPlayerViewNumber );
  }

	return;
}

//*--------------------------------------------------------------*
//* This method is the signal handler for the "About" menu item. * 
//*--------------------------------------------------------------*

void CApplication::On_About()
{ 
  // Make the AboutView view visible.

  iViewBook->set_current_page( iAboutViewNumber );
	return;
}

//*---------------------------------------------------------------------*
//* This method is the signal handler for the "Instructions" menu item. * 
//*---------------------------------------------------------------------*

void CApplication::On_Help()
{ 
	// Make the HelpView view visible.

	iViewBook->set_current_page( iHelpViewNumber );
	return;
}

//*-----------------------------------------------------------------*
//* This method is the signal handler for view pages that are done, *
//* and the only activity to perform is re-display the PlayerView.  *
//*-----------------------------------------------------------------*

void CApplication::On_View_Done()
{ 
	// Make the PlayerView view visible.

	iViewBook->set_current_page( iPlayerViewNumber );
	return;
}

//*----------------------------------------------------------------*
//* This method is the signal handler for the Window delete event, *
//* and is called if the window delete icon [X] is clicked.        * 
//*----------------------------------------------------------------*
//* RETURN: TRUE to prevent the event from propagating further.    *
//*----------------------------------------------------------------*

bool CApplication::On_Window_Delete( GdkEventAny* any_event )
{
	// Call the application quit method.

	quit();

	// Return TRUE to prevent further propagation of the event.
	
	return TRUE;
}
