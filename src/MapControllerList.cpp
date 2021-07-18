// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MapControllerList class implementation.
// The MapControllerList class manages a list of MapControllers. 
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

#include "EnigmaWC.h"
#include "MapControllerList.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CMapControllerList::CMapControllerList()
{
  return;
}

//*-----------------------------------------------------------*
//* This method changes all controllers to their saved state. *
//*-----------------------------------------------------------*

void CMapControllerList::Load()
{
  std::list<CMapController>::iterator Controller;
  
  for ( Controller = begin(); Controller != end(); ++ Controller )
  {
    (*Controller).Load();
  }
  
  return;
}

//*-------------------------------------------------*
//* This method saves the state of all controllers. *
//*-------------------------------------------------*

void CMapControllerList::Save()
{
  std::list<CMapController>::iterator Controller;
  
  for ( Controller = begin(); Controller != end(); ++ Controller )
  {
    (*Controller).Save();
  }
  
  return;
}

//*-------------------------------------------------------------*
//* This method changes all controllers to their restart state. *
//*-------------------------------------------------------------*

void CMapControllerList::Restart()
{
  std::list<CMapController>::iterator Controller;
  
  for ( Controller = begin(); Controller != end(); ++ Controller )
  {
    (*Controller).Restart();
  }
  
  return;
}
