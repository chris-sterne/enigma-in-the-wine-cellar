// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Transition class header.  The Transition class describes
// a transition path between two player locations.
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
 
#ifndef __TRANSITION_H__
#define __TRANSITION_H__

#include <gtkmm.h>
#include "EnigmaWC.h"
#include "Map.h"
#include "FinePoint.h"

class CTransition
{
  public:    
    // Public methods.

    CTransition();
    guint8 GetTotal() const;
    guint8 GetStep() const;
    void SetDivision( gint64 aDivision );
    const CFinePoint& GetPoint() const;
    void Prepare( std::list<CMapPlayer>::iterator aPlayer );
    void Advance();
    void Clear();
   
  private:
    // Private data.

    gint64 iDivision;                    // Time duration of each step.
    guint8 iTotal;                       // Total transition steps.
    guint8 iStep;                        // Current transition step.    
    CFinePoint iBegin;                   // Path begin point.
    CFinePoint iEnd;                     // Path end point.
    CFinePoint iDelta;                   // Path point change.
    CFinePoint iPoint;                   // Current path point.
};

#endif // __TRANSITION_H__
