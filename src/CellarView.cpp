// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the CellarView class implementation.  The CellarView class
// displays a startup screen.
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

#include <glibmm/i18n.h>
#include "EnigmaWC.h"
#include "MapObject.h"
#include "CellarView.h"
#include "Resources.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <gdk/gdkx.h>

//*--------------------*
//* Local declarations *
//*--------------------*

#define KVisualDelay 2              // 2 second delay.

#define KIndoorR (GLfloat)40/255    // Indoor colour: Dark grey.
#define KIndoorG (GLfloat)40/255
#define KIndoorB (GLfloat)40/255

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CCellarView::CCellarView()
{
  set_required_version(3, 2);
  set_auto_render(false);
  set_has_depth_buffer(true);
  set_use_es(false);

  // Request on_map_event signal be emitted when the widget becomes visible,
  // which will be used to trigger the startup screen.

  add_events( Gdk::STRUCTURE_MASK );
  
  // Fill all available space.

  set_hexpand( TRUE );
  set_vexpand( TRUE );
  set_can_focus( FALSE );

  // Create an image mesh for the wine cellar.

  //CResources Resources;
  //Resources.LoadImageMesh( iMesh, "WineCellar" );

	// Construct a signal slot for the visual delay timer.
	
  m_slot_timeout = sigc::mem_fun( *this, &CCellarView::On_Timeout );
	return;
}

//----------------------------------------------------
// This method is called after the widget is realized.
//----------------------------------------------------

void CCellarView::on_realize()
{
	// Pass method to parent class.  A Gtk:ERROR will occur otherwise,
	// suggesting the GLArea child class utilizes the method.
	
	Gtk::GLArea::on_realize();
	
	// Active the current OpenGL context and initialize the MeshList
	// within the context.

	//make_current();
	//iMeshList.Initialize();
}

//*---------------------------------------------------------------------*
//* This method is called when the widget is visible (has been mapped). *
//*---------------------------------------------------------------------*

bool CCellarView::on_map_event( GdkEventAny* aEvent )
{	
	// Start a one-shot visual delay timer.
	
	Glib::signal_timeout()
		.connect_seconds_once(slot_timeout(),
		                      KVisualDelay,
		                      Glib::PRIORITY_DEFAULT );	
	return FALSE;
}

//*-----------------------------------*
//* Method to draw CellarView widget. *
//*-----------------------------------*

//gboolean CCellarView::on_render()
bool CCellarView::on_render(const Glib::RefPtr<Gdk::GLContext>& context)
{  
  // Clear the widget area.
	
  glClearColor( KIndoorR, KIndoorG, KIndoorB, -1 );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
  // Prepare WineCellar object.
  
	CMapObject cellar;
	cellar.iID = EnigmaWC::ID::EWineCellar;
	cellar.iSurface = EnigmaWC::Direction::EBelow;
	cellar.iRotation = EnigmaWC::Direction::ESouth;
	cellar.iLocation.Clear();
	cellar.iLocation.iNorth = 1;
  cellar.iSense.SetState( FALSE );
  cellar.iState.SetState( TRUE );
  cellar.iVisibility.SetState( TRUE );
  cellar.iPresence.SetState( TRUE );

	// Prepare viewer of WineCeller object.

	CMapPlayer viewer;
	viewer.iID = EnigmaWC::ID::EPlayer;
	viewer.iSurface = EnigmaWC::Direction::EBelow;
	viewer.iRotation = EnigmaWC::Direction::ENorth;
	viewer.iLocation.Clear();
	viewer.iOffset.iNorth = 0.2f;
  
  // Render the WineCellar object.
  
  iMeshList.Render(cellar, viewer);
  return TRUE;
}

//*--------------------------------------------------------------*
//* This method is called when the visual delay timer times out. *
//*--------------------------------------------------------------*

void CCellarView::On_Timeout()
{
  m_signal_done.emit();
	return;
}

//*--------------------------------------------------*
//* This method returns the view done signal server. *
//*--------------------------------------------------*

CCellarView::type_signal_done CCellarView::signal_done()
{
  return m_signal_done;
}

//*-----------------------------------------------------------*
//* This method returns the visual delay timeout signal slot. *
//*-----------------------------------------------------------*

CCellarView::type_slot_timeout CCellarView::slot_timeout()
{
  return m_slot_timeout;
}
