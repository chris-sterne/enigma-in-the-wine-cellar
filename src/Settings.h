/*--------------------------------------------*
 * Program: Enigma in the Wine Cellar         *
 * Version: 5.0 for Linux OS                  *
 * File:    AboutView.cpp                     *
 * Date:    September 17, 2016                *
 * Author:  Chris Sterne                      *
 *                                            *
 * Settings class header.                     *
 *--------------------------------------------*/

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <glibmm/i18n.h>
#include <gtkmm.h>

class CSettings : public Glib::KeyFile
{
  public:
    // Action Key group

    constexpr static const char* ACTION_KEY_GROUP = "Action Keys";
    constexpr static const char* MOVE_FORWARD     = "Move Forward";
    constexpr static const char* MOVE_BACKWARD    = "Move Backward";
    constexpr static const char* MOVE_LEFT        = "Move Left";
    constexpr static const char* MOVE_RIGHT       = "Move Right";
    constexpr static const char* TURN_LEFT        = "Turn Left";
    constexpr static const char* TURN_RIGHT       = "Turn Right";
    constexpr static const char* INVENTORY        = "Inventory";
    constexpr static const char* SWITCH_PLAYER    = "Switch Player";
		
    // Miscellaneous group

    constexpr static const char* MISCELLANEOUS_GROUP = "Miscellaneous";
    constexpr static const char* STARTUP_SCREEN      = "Startup Screen";
    constexpr static const char* VIEW_TRANSITIONS    = "View Transitions";
    constexpr static const char* FULL_ORIENTATION    = "Full Orientation";
    constexpr static const char* GAME_SOUNDS         = "Game Sounds";

    // Game map group

    constexpr static const char* GAME_MAP_GROUP = "Game Map";
    constexpr static const char* GAME_MAP       = "Game Map";
		
    // Public methods.

    CSettings();
    void Load();
    void Save();
};

#endif // __SETTINGS_H__