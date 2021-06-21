/*-----------------------------------------------------*
 * Program: Enigma in the Wine Cellar                  *
 * Version: 3.0 for Linux OS                           *
 * File:    CellarView.cpp                             *
 * Date:    October 21, 2016                           *
 * Author:  Chris Sterne                               *
 *                                                     *
 * CellarView class header.                            *
 *-----------------------------------------------------*
 * This class displays the startup wine cellar screen. *
 *-----------------------------------------------------*/

#include <glibmm/i18n.h>
#include "CellarView.h"
#include "Resources.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <gdk/gdkx.h>

//*--------------------*
//* Local declarations *
//*--------------------*

#define KVisualDelay 2              // 2 second delay.

#define KIndoorR (GLfloat)30/255    // Dark indoor colour: Dark grey.
#define KIndoorG (GLfloat)30/255
#define KIndoorB (GLfloat)30/255

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CCellarView::CCellarView()
{
  // Request on_map_event signal be emitted when the widget becomes visible,
  // which will be used to trigger the startup screen.

  add_events( Gdk::STRUCTURE_MASK );
  
  // Fill all available space.

  set_hexpand( TRUE );
  set_vexpand( TRUE );
  set_can_focus( FALSE );

  // Create an image mesh for the wine cellar.

  CResources Resources;
  Resources.LoadImageMesh( iMesh, "WineCellar" );

	// Construct a signal slot for the visual delay timer.
	
  m_slot_timeout = sigc::mem_fun( *this, &CCellarView::On_Timeout );


	return;
}

//*---------------------------------------------------------------------*
//* This method is called when the widget is visible (has been mapped). *
//*---------------------------------------------------------------------*

bool CCellarView::on_map_event( GdkEventAny* aEvent )
{
	// Start a one-shot visual delay timer.
	
	Glib::signal_timeout().connect_seconds_once( slot_timeout(),
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
	
  // Exit if there is no wine cellar image mesh, or if the mesh is empty
  // (possibly due to an error in the resource data).
	
  if ( iMesh.iActive.iVertices.empty() )
    return TRUE;

  // Prepare OpenGL matrices.

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum( -.65, .65, -.65, .65, .65, 3 );
	
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef( 0, -0.8, -1.8 );
	glRotatef( -90, 1, 0, 0 );
	glRotatef( 180, 0, 0, 1 );
	
  // The object is now positioned properly in the model space.
  // Set the object mesh information before drawing.
	
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
	
  glVertexPointer( 3, GL_FLOAT, 0, iMesh.iActive.iVertices.data() );
  glColorPointer( 3, GL_FLOAT, 0, iMesh.iActive.iColours.data() );
	
  glDrawElements( GL_TRIANGLES,
                  iMesh.iActive.iFaces.size(),
	                GL_UNSIGNED_INT,
                  iMesh.iActive.iFaces.data() );
	
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

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
