// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the GLArea class header.  The GLArea class provides an area
// for OpenGL drawing.
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

#ifndef __GLAREA_H__
#define __GLAREA_H__

#include <gtkmm.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <gdk/gdkx.h>

class CGLArea : public Gtk::Widget
{
  public:
    // Public methods.

    CGLArea();
    ~CGLArea();
    
    // Virtual methods.
      
    virtual gboolean on_render();
    virtual void queue_render();
		
  protected:
		// Overridden base class methods.
		
		void on_size_allocate( Gtk::Allocation& allocation );
		void on_map();
		void on_realize();
		void on_unrealize();
    bool on_draw( const Cairo::RefPtr<Cairo::Context>& aCC );
		virtual Gtk::SizeRequestMode get_request_mode_vfunc() const;
		
  private:		
		// Private data.

		Glib::RefPtr<Gdk::Window> iWindow;   // GdkWindow used for drawing.
		Display* iXDisplay;                  // X11 display associated with Widget.
		int iXScreen;                        // X11 screen associated with Widget.
		XVisualInfo* iXVisual;               // X11 visual associated with Widget.
		Window iXID;                         // X11 Window ID.
		GLXContext iGLXContext;              // GLX context.
};

#endif // __GLAREA_H__
