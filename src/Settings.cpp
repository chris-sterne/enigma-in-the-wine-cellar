// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Settings class implementation.  The Settings class
// manages the application settings file.
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

#include "Settings.h"

//*-----------------------------*
//* Local declarations and data *
//*-----------------------------*

static const std::string KSettingsFilename = "./EnigmaWC.ini";

static const char* TitleComment = "# Enigma in the Wine Cellar v5.0 settings #";

static const char* ActionKeyGroup = CSettings::ACTION_KEY_GROUP;

struct ActionKeyInfo
{
  const char* iKeyName;
  const int iKeyValue;
};

static ActionKeyInfo ActionKeys[] =
{
  { CSettings::MOVE_FORWARD, GDK_KEY_KP_Up },
  { CSettings::MOVE_BACKWARD, GDK_KEY_KP_Begin },
  { CSettings::MOVE_LEFT, GDK_KEY_KP_Home },
  { CSettings::MOVE_RIGHT, GDK_KEY_KP_Page_Up },  
  { CSettings::TURN_LEFT, GDK_KEY_KP_Left },
  { CSettings::TURN_RIGHT, GDK_KEY_KP_Right },
  { CSettings::INVENTORY,  GDK_KEY_i },
  { CSettings::SWITCH_PLAYER, GDK_KEY_p },
  { NULL, 0 }
};

static const char* MiscellaneousGroup = CSettings::MISCELLANEOUS_GROUP;

struct MiscellaneousInfo
{
  const char* iKeyName;
  const char* iKeyValue;
};

static MiscellaneousInfo Miscellaneous[] =
{
  { CSettings::STARTUP_SCREEN, "true" },
  { CSettings::VIEW_TRANSITIONS, "true" },
  { CSettings::FULL_ORIENTATION, "false" },
  { CSettings::GAME_SOUNDS, "false" },
  { NULL, NULL }
};

static const char* GameMapGroup   = CSettings::GAME_MAP_GROUP;
static const char* GameMapKeyName = CSettings::GAME_MAP;
static const char* GameMapKeyValue = "First.ewc";

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CSettings::CSettings()
{
  return;
}

//*---------------------------------------------------*
//* This method load settings from the settings file. *
//*---------------------------------------------------*

void CSettings::Load()
{
  try
  {
    load_from_file( KSettingsFilename, Glib::KEY_FILE_KEEP_COMMENTS );
  }
  catch( Glib::Error error )
  {
    // If the settings file could not be loaded, prepare default settings.

    Glib::ustring String;
    Glib::ustring SubString;

    // Start with a title comment.

    String.append( TitleComment );
    String.append( "\n");
		
    // Create and fill a group for the action keys.
		
    SubString = Glib::ustring::compose( "\n[%1]\n", ActionKeyGroup );
    String.append( SubString );

    for ( int Index = 0; ActionKeys[ Index ].iKeyName != NULL; Index ++ )
    {
      SubString =
        Glib::ustring::compose( "%1 = %2\n",
                                ActionKeys[ Index ].iKeyName,
                                ActionKeys[ Index ].iKeyValue );

      String.append( SubString );
		}

    // Create and fill a group for miscellaneous settings.

    SubString = Glib::ustring::compose( "\n[%1]\n", MiscellaneousGroup );
    String.append( SubString );
		
    for ( int Index = 0; Miscellaneous[ Index ].iKeyName != NULL; Index ++ )
    {
      SubString =
        Glib::ustring::compose( "%1 = %2\n",
                                Miscellaneous[ Index ].iKeyName,
                                Miscellaneous[ Index ].iKeyValue );

      String.append( SubString );
    }

    // Create and fill a group for the game map filename.

    SubString = Glib::ustring::compose( "\n[%1]\n", GameMapGroup );
    String.append( SubString );

    SubString =
      Glib::ustring::compose( "%1 = %2\n",
                              GameMapKeyName,
                              GameMapKeyValue );

    String.append( SubString );
    String.append( "\n");

    // Fill the KeyFile from the string.
		
    load_from_data( String, Glib::KEY_FILE_KEEP_COMMENTS );
  }

  return;
}

//*--------------------------------------------------*
//* This method saves settings to the settings file. *
//*--------------------------------------------------*

void CSettings::Save()
{	
  save_to_file( KSettingsFilename );	
  return;
}
