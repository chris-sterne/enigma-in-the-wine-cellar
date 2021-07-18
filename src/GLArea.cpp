// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the GLArea class implementation.  The GLArea class
// provides an area for OpenGL drawing.
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

#include "GLArea.h"
#include "ErrorDialog.h"

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CGLArea::CGLArea()
{	
	set_has_window( TRUE );
	set_double_buffered( FALSE );

	// Initialize instance data.

	iXDisplay   = NULL;
	iXScreen    = 0;
	iGLXContext = NULL;

	return;
}

//*-------------------------*
//* C++ default destructor. *
//*-------------------------*

CGLArea::~CGLArea()
{
  return;
}

//*-------------------------------------------*
//* Method to get request mode of the widget. *
//*-------------------------------------------*

Gtk::SizeRequestMode CGLArea::get_request_mode_vfunc() const
{
  //Accept the default value supplied by the base class.

	return Gtk::Widget::get_request_mode_vfunc();
}

//*------------------------------------------------------------------*
//* This method is called when the widget has been allocated a size. *
//*------------------------------------------------------------------*

void CGLArea::on_size_allocate( Gtk::Allocation& allocation )
{
	// Use the offered allocation for this container:

	set_allocation(allocation);

	if ( iWindow && iGLXContext )
  {	
		iWindow->move_resize( allocation.get_x(),
		                      allocation.get_y(),
                          allocation.get_width(),
                          allocation.get_height() );

    // From observation, not calling process_updates() results in the exposed
    // window areas being improperly drawn during the first rendering after a 
    // window resize (as if a temporary geometry mismatch exists between the
    // window and the OpenGL ViewPort).  This does not occur in OpenGL
    // double-buffer OpenGL mode.
    
    iWindow->process_updates( FALSE );
    
    // Make the OpenGL context current, and set the ViewPort geometry.

    glXMakeCurrent( iXDisplay, iXID, iGLXContext);
    glViewport( 0, 0, allocation.get_width(), allocation.get_height());
  }

	return;
}

//*-------------------------------------------------------------*
//* This method is called when the widget is about to be shown. *
//*-------------------------------------------------------------*

void CGLArea::on_map()
{
  // Pass the method to the base class.

  Gtk::Widget::on_map();
  return;
}

//*----------------------------------------------------------*
//* This method is called when the widget has been realized. *
//*----------------------------------------------------------*

void CGLArea::on_realize()
{
  set_realized();

  // Create a Gdk::Window for drawing.

  if ( !iWindow )
  {
    GdkWindowAttr attributes;
    memset(&attributes, 0, sizeof(attributes));

    Gtk::Allocation allocation = get_allocation();

    // Set initial position and size of the Gdk::Window.
		
    attributes.x = allocation.get_x();
    attributes.y = allocation.get_y();
    attributes.width = allocation.get_width();
    attributes.height = allocation.get_height();

    attributes.event_mask = get_events () | Gdk::EXPOSURE_MASK;
    attributes.window_type = GDK_WINDOW_CHILD;
    attributes.wclass = GDK_INPUT_OUTPUT;

    iWindow =
			Gdk::Window::create( get_parent_window(),
			                     &attributes,
          								 GDK_WA_X | GDK_WA_Y );
    set_window( iWindow );

    // Make the widget receive expose events.

    iWindow->set_user_data(gobj());
  }

  // Initialize OpenGL Context from X11 resources.
	
  Glib::RefPtr<Gdk::Display> display = get_display();
  iXDisplay = gdk_x11_display_get_xdisplay( display->gobj() );

  Glib::RefPtr<Gdk::Screen> screen = get_screen();	
  iXScreen = gdk_x11_screen_get_screen_number(screen->gobj() );

  int XAttributes[] =
  {
	  GLX_RGBA,
    //GLX_DOUBLEBUFFER,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_DEPTH_SIZE, 24,
    GLX_STENCIL_SIZE, 8,
    None
  };
           
  iXVisual = glXChooseVisual( iXDisplay, iXScreen, XAttributes);

  if ( iXVisual )
  {
    // Create an OpenGL context.

    iGLXContext = glXCreateContext( iXDisplay, iXVisual , NULL, TRUE );
    GtkWidget* Widget = (GtkWidget *)gobj();
    GdkWindow* drawable = gtk_widget_get_window(Widget);
    iXID = gdk_x11_window_get_xid( drawable );
	
    // Initialize the OpenGL context.
    
    glXMakeCurrent( iXDisplay, iXID, iGLXContext);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_DITHER);
    glShadeModel(GL_FLAT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClear( GL_COLOR_BUFFER_BIT );
  }

  // Display an error requester if an OpenGL context could not be created.
  
  if ( iGLXContext == NULL )
  {
    CErrorDialog ErrorDialog;
    ErrorDialog.Run( CErrorDialog::ID::ENoOpenGL );
  }

  return;
}

//*------------------------------------------------------------*
//* This method is called when the widget has been unrealized. *
//*------------------------------------------------------------*

void CGLArea::on_unrealize()
{	
  // Destroy the OpenGL context.
	
  glXDestroyContext( iXDisplay, iGLXContext );
  iWindow.reset();
	
  // Pass the method to the base class.
	
  Gtk::Widget::on_unrealize();  
  return;
}

//*--------------------------------------*
//* Method to queue a rendering request. *
//*--------------------------------------*

void CGLArea::queue_render()
{
  queue_draw();
  return;
}

//*----------------------------*
//* Method to draw the widget. *
//*----------------------------*

bool CGLArea::on_draw( const Cairo::RefPtr<Cairo::Context>& aCC )
{  
  // Make the OpenGL context current.

  glXMakeCurrent( iXDisplay, iXID, iGLXContext );
	
  // Perform OpenGL rendering.  If requested, perform a buffer swap to update
  // the view (use glFlush() instead for single-buffer OpenGL modes).

  if ( on_render() )
    //glXSwapBuffers( iXDisplay, iXID );
    glFlush();

  return TRUE;
}

//*-----------------------------------------------*
//* Default implementation of on_render() method. *
//*-----------------------------------------------*

gboolean CGLArea::on_render()
{
  glClearColor( 0, 0, 0, -1 );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  return TRUE;
}
