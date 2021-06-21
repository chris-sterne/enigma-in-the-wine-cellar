/*------------------------------------------------------*
 * Program: Enigma in the Wine Cellar                   *
 * Version: 1.0 for Linux OS                            *
 * File:    CellarView.cpp                              *
 * Date:    October 21, 2016                            *
 * Author:  Chris Sterne                                *
 *                                                      *
 * CellarView class header.                             *
 *------------------------------------------------------*/

#ifndef __CELLARVIEW_H__
#define __CELLARVIEW_H__

#include <gtkmm.h>
#include "GLArea.h"
#include "Mesh.h"

class CCellarView : public Gtk::GLArea //public CGLArea
{
  public:
    // Public methods.

    CCellarView();

    // View done signal accessor.
		
    typedef sigc::signal<void> type_signal_done;
    type_signal_done signal_done();

    // Display delay timeout slot accessor.
		
    typedef sigc::slot<void> type_slot_timeout;
    type_slot_timeout slot_timeout();

  private:
    // Private methods.

    void On_Timeout();
		
  protected:
    // Overridden base class methods.
		
    bool on_map_event(GdkEventAny* aEvent );
    //gboolean on_render();
    bool on_render(const Glib::RefPtr<Gdk::GLContext>& context) override;

  private:
    // Private data.

    CMesh iMesh;                        // Wine cellar image mesh.
    type_signal_done m_signal_done;     // View done signal server.
    type_slot_timeout m_slot_timeout;   // Visual delay timeout signal slot.
};

#endif // __CELLARVIEW_H__
