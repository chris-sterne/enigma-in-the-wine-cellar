//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 4.0 for Linux OS          *
//* File:    Application.h             *
//* Date:    September 17, 2016        *
//* Author:  Chris Sterne              *
//*                                    *
//* Application class header.          *
//*------------------------------------*
 
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <gtkmm.h>
#include "Settings.h"
#include "Window.h"
#include "Map.h"
#include "CellarView.h"
#include "PlayerView.h"
#include "InventoryView.h"
#include "MapsView.h"
#include "AboutView.h"
#include "SettingsView.h"
#include "HelpView.h"
#include "GameDialog.h"

class CApplication : public Gtk::Application
{
  public:
    // Public methods.
		
    CApplication();

    void On_Game_Load();
    void On_Game_Save();
    void On_Game_Restart();
    void On_View_Done();
    void On_Inventory();
    void On_Switch();
    void On_Item_Selected( std::list<CMapItem>::iterator aItem );
    void On_Maps();
    void On_Map_Load( const std::string aFilename );
    void On_About();
    void On_Help();
    void On_Settings();
    void On_Settings_Done();
    bool On_Window_Delete( GdkEventAny* any_event );
    void On_Shutdown();
		
  protected:
    // Protected methods from base class.

    void on_activate();

  private:
    // Private data.

    std::unique_ptr< CSettings > iSettings;           // Application settings.
    std::unique_ptr< CGameDialog > iGameDialog;       // Game dialog.
		std::unique_ptr<CWindow> iWindow;                 // Application window.
		std::shared_ptr<CMap> iMap;                       // Game map.
		std::unique_ptr<Gtk::Notebook> iViewBook;         // View page handler.
		std::unique_ptr<CCellarView> iCellarView;         // CellarView widget.
		std::unique_ptr<CPlayerView> iPlayerView;         // PlayerView widget.
    std::unique_ptr< CInventoryView > iInventoryView; // InventoryView widget.
    std::unique_ptr<CMapsView> iMapsView;             // MapsView widget.
    std::unique_ptr<CAboutView> iAboutView;           // AboutView widget.
    std::unique_ptr<CHelpView> iHelpView;             // HelpView widget.
    std::unique_ptr< CSettingsView > iSettingsView;   // SettingsView widget.
    std::unique_ptr< Gtk::MenuItem> iLoadMenuItem;    // Game Load menu item.
    std::unique_ptr< Gtk::MenuItem> iSaveMenuItem;    // Game Save menu item.
    std::unique_ptr< Gtk::MenuItem> iRestartMenuItem; // Game Restart menu item.
    std::unique_ptr< Gtk::MenuItem> iSwitchMenuItem;  // Player switch menu item.
    int iCellarViewNumber;                            // CellarView page number.
		int iPlayerViewNumber;                            // PlayerView page number.
    int iInventoryViewNumber;                         // InventoryView page number.
    int iMapsViewNumber;                              // MapsView page number.
    int iAboutViewNumber;                             // AboutView page number.
    int iSettingsViewNumber;                          // SettingsView page number.
    int iHelpViewNumber;                              // HelpView page number.
    gboolean iStartupScreen;                          // TRUE to show startup screen.
};

#endif // __APPLICATION_H__