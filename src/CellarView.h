// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the CellarView class header.  The CellarView class displays
// a startup screen. 
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

#ifndef __CELLARVIEW_H__
#define __CELLARVIEW_H__

#include <gtkmm.h>
#include "GLArea.h"
#include "MeshList.h"

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
		
    bool on_map_event(GdkEventAny* aEvent ) override;
    bool on_render(const Glib::RefPtr<Gdk::GLContext>& context) override;
		void on_realize() override;

  private:
    // Private data.

    CMeshList iMeshList;
    type_signal_done m_signal_done;     // View done signal server.
    type_slot_timeout m_slot_timeout;   // Visual delay timeout signal slot.
};

#endif // __CELLARVIEW_H__
