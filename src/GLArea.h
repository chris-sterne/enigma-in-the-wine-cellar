//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 3.0 for Linux OS          *
//* File:    GLArea.cpp                *
//* Date:    July 27, 2016             *
//* Author:  Chris Sterne              *
//*                                    *
//* GLArea class header.               *
//*------------------------------------*

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
