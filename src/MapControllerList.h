// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MapControllerList class header.  The MapControllerList
// class manages a list of MapControllers. 
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
 
#ifndef __MAPCONTROLLERLIST_H__
#define __MAPCONTROLLERLIST_H__

#include <gtkmm.h>
#include "MapController.h"

class CMapControllerList : public std::list<CMapController>
{
  public:
    // Public methods.

    class CSignalIndex
    {
      public:
        guint16 iController;   // Index to controller.
        guint16 iSignal;       // Index to signal in controller.
    };

    CMapControllerList();
    void Load();
    void Save();
    void Restart();
};

#endif // __MAPCONTROLLERLIST_H__
