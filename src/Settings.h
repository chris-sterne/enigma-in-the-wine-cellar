// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Settings class header.  The Settings class manages
// the application settings file.
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
