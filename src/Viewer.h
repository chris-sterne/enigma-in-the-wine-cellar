// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Viewer class header.  A Viewer displays the view from
// a location within a game world.
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

#ifndef __ENIGMA_VIEWER_H__
#define __ENIGMA_VIEWER_H__

#include <GL/gl.h>
#include <gdkmm/glcontext.h>
#include <sigc++/sigc++.h>
#include "Matrix4.h"
#include "FineLocation.h"
#include "ObjectList.h"
#include "ItemList.h"
#include "Boundary.h"

namespace Enigma
{
	// The Viewer inherits the sigc::trackable interface so signal
	// connections to the Viewer are automatically disconnected at
	// their source when the Viewer is destroyed.

	class Viewer : public sigc::trackable
	{
		public:
		// Public declarations.
			
			enum class Direction    // View space directions.
			{
				NONE = 0,
				FRONT,
				BACK,
				RIGHT,
				LEFT,
				UP,
				DOWN,
				CENTER,
				TOTAL
			};
		
		  // Public methods.

		  Viewer(Enigma::Object::ID id);
		  void set_objects(const std::shared_ptr<Enigma::ObjectList> objects);
		  void set_items(const std::shared_ptr<Enigma::ItemList> items);
			void set_boundary(const std::shared_ptr<Enigma::Boundary> boundary);
		  void update();
			bool render(const Glib::RefPtr<Gdk::GLContext>& context);
			void do_location();
			void clear_motion();
		  void set_turn_left();
		  void set_turn_right();
		  void set_move_one(Enigma::Location::Direction direction);
		  void set_move_one(Enigma::Viewer::Direction direction);
			void set_rotation(Enigma::Location::Direction direction);
			void set_location(Enigma::Location location);

			// "queue_render" signal type and server accessor.

			typedef sigc::signal<void> type_signal_queue_render;	
			type_signal_queue_render signal_queue_render();
			
			// "location" signal type and server accessor.

			typedef sigc::signal<void, Enigma::Location&> type_signal_location;	
			type_signal_location signal_location();
			
			// "update done" slot type and slot accessor.
			
			typedef sigc::slot<void> type_slot_update_done;
			type_slot_update_done& slot_update_done();

			// Public data.  For convenience, m_location is declared externally
			// from m_object to avoid repetitive m_object->m_location usage,
			// but this means the m_object location must be updated before
			// it is rendered.
			
			std::shared_ptr<Enigma::Object> m_object;
			Enigma::Location m_location;
			Enigma::Location m_next_location;
			
			// World information.
			
			std::shared_ptr<Enigma::ObjectList> m_world_objects;
			std::shared_ptr<Enigma::ItemList> m_world_items;
			std::shared_ptr<Enigma::Boundary> m_world_boundary;

		private:
			// Private methods.
					
			void render_viewcone();
			void normalize_plane(GLfloat& a, GLfloat& b, GLfloat& c, GLfloat& d);
			
			void adjust_boundary(GLfloat i0, GLfloat j0, GLfloat k0,
  		                     GLfloat i1, GLfloat j1, GLfloat k1);
		
			void adjust_boundary(GLfloat magnitude,
			                     GLfloat i, GLfloat j, GLfloat k);
		
			void adjust_boundary(GLfloat offset, GLfloat& low, GLfloat& high);
		
			// Private data.
			
			type_signal_queue_render m_signal_queue_render;
			type_signal_location m_signal_location;
			type_slot_update_done m_slot_update_done;
	
			gint64 m_old_time;              // Last recorded monotonic time.
			Enigma::FineLocation m_point;   // Current point.
			
			// Motion request relative offsets.
			
			short m_translate_east;
			short m_translate_above;
			short m_translate_north;
			short m_rotate_east;
			short m_rotate_above;
			short m_rotate_north;
			
			// Viewcone object buffers.
			
			std::vector<std::shared_ptr<Enigma::Object>> m_objects;
			std::vector<std::shared_ptr<Enigma::Item>> m_items;
			
			// Viewcone in standard position.

			Enigma::Matrix4 m_viewcone;
			
			// Distances from camera point to near and far clipping planes along
			// the intersection of the clipping planes.  These distances allow
			// calculating frustrum corner points once the unit vector pointing
			// along the intersection of two clipping planes is known.

			GLfloat m_near_edge;
			GLfloat m_far_edge;
			
			// Fine-resolution world-axis-aligned boundary of viewcone frustrum,
			// relative to the viewer's position.  These change as the viewer
			// rotates or translates.

			GLfloat m_box_low_x;
			GLfloat m_box_high_x;
			GLfloat m_box_low_y;
			GLfloat m_box_high_y;
			GLfloat m_box_low_z;
			GLfloat m_box_high_z;
	};
}

#endif // __ENIGMA_VIEWER_H__
