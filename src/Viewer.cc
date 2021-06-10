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

#include "Viewer.h"

//-------------------
// Local declarations
//-------------------

// Transition timings (microseconds).

static const float TRANSITION_DURATION = 400000.0f;

// Indoor background colour (dark grey).

static const GLfloat INDOOR_R = 0.156f;
static const GLfloat INDOOR_G = 0.156f;
static const GLfloat INDOOR_B = 0.156f;

// Viewcone perspective projection values.

static const GLfloat NEAR_CLIP     = 1.1f;
static const GLfloat FAR_CLIP      = 8.0f;
static const GLfloat FIELD_OF_VIEW = 55.0f;
static const GLfloat ASPECT_RATIO  = 1.0f;

// Scale factor between East,Above,North and X,Y,Z coordinate spaces.

static const GLfloat FULL_ROOM = 2.0f;    // Width of a world room.
static const GLfloat HALF_ROOM = 1.0f;    // Room center to outer walls.

// A world space room has a distance of 1.0 from its center to the outer
// walls, so a bounding sphere around the room will have a radius of
// sqrt(2.0).

static const GLfloat BOUNDING_RADIUS = 1.4142f;

// Constant to add onto fine world positions before converting them
// into coarse world positions.  This triggers a rounding-up to occur
// before the viewer is able to move outside the position boundary.

static const GLfloat ROUND_UP = 0.4f;

// Constant to limit visible distance from viewer, giving a sense of
// "curvature" at the far limit.  Using a squared value avoids having
// to take the square root.

static const GLfloat FAR_RADIUS_SQUARED = 64.0f;
   
//------------------------------------------------------------------
// This constructor has an Object ID argument that is used to create
// the Object being turned into a viewer.
//------------------------------------------------------------------
// id: Object ID.
//------------------------------------------------------------------

Enigma::Viewer::Viewer(Enigma::Object::ID id)
{	
	// Create object to act as a viewer.

	m_object = Enigma::Object::create(id);

	// Clear motion request variables.
	
	clear_motion();
	
	// Calculate the distance along the intersection the clipping planes
	// (left, right, bottom, top) from the camera to a point on the near
	// and far clipping planes.
	
	GLfloat height = FAR_CLIP * tan(FIELD_OF_VIEW * 3.14159f / 360.0f);
	GLfloat width  = height * ASPECT_RATIO;
	
	m_far_edge =
		sqrt((height * height) + (width * width) + (FAR_CLIP * FAR_CLIP));

	m_near_edge = m_far_edge * (NEAR_CLIP / FAR_CLIP);
	
	// Prepare a viewcone projection matrix in a standard orientation
	// (viewer facing north).  A copy of this matrix will be made and
	// transformed as needed, which is quicker than having to prepare
	// the matrix every time.
	
	m_viewcone.perspective(FIELD_OF_VIEW, ASPECT_RATIO, NEAR_CLIP, FAR_CLIP);
}

//--------------------------------------------
// This method sets the list of world objects.
//--------------------------------------------

void Enigma::Viewer::set_objects(
	const std::shared_ptr<Enigma::ObjectList> objects)
{
	m_world_objects = objects;
}

//--------------------------------------------
// This method sets the list of world items.
//--------------------------------------------

void Enigma::Viewer::set_items(
	const std::shared_ptr<Enigma::ItemList> items)
{
	m_world_items = items;
}

//-------------------------------------
// This method sets the world boundary.
//-------------------------------------

void Enigma::Viewer::set_boundary(
	const std::shared_ptr<Enigma::Boundary> boundary)
{
	m_world_boundary = boundary;
}

//----------------------------------------------
// This method clears all motion request values.
//----------------------------------------------

void Enigma::Viewer::clear_motion()
{
	// Clear motion request values.  When these are all zero,
	// a transition will not begin during a view rendering.
	
	m_translate_east  = 0;
	m_translate_above = 0;
	m_translate_north = 0;
	m_rotate_east     = 0;
	m_rotate_above    = 0;
	m_rotate_north    = 0;
}

//----------------------------------------------------
// This method sets a one position move in view space.
//----------------------------------------------------
// direction: View space direction of move.
//----------------------------------------------------

void Enigma::Viewer::set_move_one(Enigma::Viewer::Direction direction)
{
	// Get surface and rotation of the viewer.
	
	Enigma::Location::Direction surface  = m_location.m_surface;
	Enigma::Location::Direction rotation = m_location.m_rotation;
	
	// Convert view space direction into world space direction,
	// then set a one position move in world space.
	
	switch (direction)
	{
		case Enigma::Viewer::Direction::FRONT:
			set_move_one(rotation);
			break;
			
		case Enigma::Viewer::Direction::BACK:
			set_move_one(Enigma::Location::get_opposite(rotation));
			break;
			
		case Enigma::Viewer::Direction::RIGHT:
			set_move_one(Enigma::Location::get_right(surface, rotation));
			break;
			
		case Enigma::Viewer::Direction::LEFT:
			set_move_one(Enigma::Location::get_left(surface, rotation));
			break;
			
		case Enigma::Viewer::Direction::UP:
			set_move_one(Enigma::Location::get_opposite(surface));
			break;
			
		case Enigma::Viewer::Direction::DOWN:
			set_move_one(surface);
			break;
		
		default:
			break;
	}
}

//-----------------------------------------------------
// This method sets a one position move in world space.
//-----------------------------------------------------
// direction: World space direction of move.
//-----------------------------------------------------

void Enigma::Viewer::set_move_one(Enigma::Location::Direction direction)
{
	// Set a one position offset in world space unless at a boundary. 
	
	switch (direction)
	{
		case Enigma::Location::Direction::NORTH:
			// Move one position north if possible.
			
			if (m_location.m_north < 65535)
			{
				m_translate_north = 1;
				m_next_location.m_north += m_translate_north;
			}
			
			break;
			
		case Enigma::Location::Direction::SOUTH:
			// Move one position south if possible.
			
			if (m_location.m_north > 0)
			{
				m_translate_north = -1;
				m_next_location.m_north += m_translate_north;
			}
			
			break;
		
		case Enigma::Location::Direction::EAST:
			// Move one position east if possible.
			
			if (m_location.m_east < 65535)
			{
				m_translate_east = 1;
				m_next_location.m_east += m_translate_east;
			}
			
			break;
		
		case Enigma::Location::Direction::WEST:
			// Move one position west if possible.
			
			if (m_location.m_east > 0)
			{
				m_translate_east = -1;
				m_next_location.m_east += m_translate_east;
			}
			
			break;
			
		case Enigma::Location::Direction::ABOVE:
			// Move one position above if possible.
			
			if (m_location.m_above < 65535)
			{
				m_translate_above = 1;
				m_next_location.m_above += m_translate_above;
			}
			
			break;
			
		case Enigma::Location::Direction::BELOW:
			// Move one position below if possible.
			
			if (m_location.m_above > 0)
			{
				m_translate_above = -1;
				m_next_location.m_above += m_translate_above;
			}
			
			break;

		default:
			break;
	}
}

//---------------------------------------------------
// This method sets a left turn motion in view space.
//---------------------------------------------------

void Enigma::Viewer::set_turn_left()
{
	// Get the current surface of the viewer.

	Enigma::Location::Direction surface = m_location.m_surface;

	// A left turn changes the viewer rotation but not its surface.

	m_next_location.m_rotation =
		Enigma::Location::get_left(surface, m_location.m_rotation);
	
	m_next_location.m_surface = surface;
	
	// Set rotation around appropriate world spaceaxis. 
	
	switch (surface)
	{
		case Enigma::Location::Direction::NORTH: m_rotate_north = -90; break;
		case Enigma::Location::Direction::SOUTH: m_rotate_north = 90;  break;
		case Enigma::Location::Direction::EAST:  m_rotate_east  = -90; break;
		case Enigma::Location::Direction::WEST:  m_rotate_east  = 90;  break;
		case Enigma::Location::Direction::ABOVE: m_rotate_above = -90; break;
		case Enigma::Location::Direction::BELOW: m_rotate_above = 90;  break;
		default: break;
	}
}

//----------------------------------------------------
// This method sets a right turn motion in view space.
//----------------------------------------------------

void Enigma::Viewer::set_turn_right()
{
	// Get the current surface of the viewer.

	Enigma::Location::Direction surface = m_location.m_surface;

	// A right turn changes the viewer rotation but not its surface.

	m_next_location.m_rotation =
		Enigma::Location::get_right(surface, m_location.m_rotation);
	
	m_next_location.m_surface = surface;
	
	// Set rotation around appropriate world space axis. 
	
	switch (surface)
	{
		case Enigma::Location::Direction::NORTH: m_rotate_north = 90;  break;
		case Enigma::Location::Direction::SOUTH: m_rotate_north = -90; break;
		case Enigma::Location::Direction::EAST:  m_rotate_east  = 90;  break;
		case Enigma::Location::Direction::WEST:  m_rotate_east  = -90; break;
		case Enigma::Location::Direction::ABOVE: m_rotate_above = 90;  break;
		case Enigma::Location::Direction::BELOW: m_rotate_above = -90; break;
		default: break;
	}
}

//--------------------------------------------
// This method sets a rotation in world space.
//--------------------------------------------
// rotation: World direction of rotation.
//--------------------------------------------

void Enigma::Viewer::set_rotation(Enigma::Location::Direction direction)
{
	// Set an immediate change in the viewer's rotation.  This will become
	// visible when the next view frame is rendered.
	
	m_location.m_rotation      = direction;
	m_next_location.m_rotation = direction;
	
	// Emit the location change in a signal.
	
	m_signal_location.emit(m_location);
}

//--------------------------------------------
// This method sets a location in world space.
//--------------------------------------------
// location: New location.
//--------------------------------------------

void Enigma::Viewer::set_location(Enigma::Location location)
{
	// Set an immediate change in the viewer's location.  This will
	// become visible when the next view frame is rendered.
	
	m_location      = location;
	m_next_location = location;
	
	// Emit the location change in a signal.
	
	m_signal_location.emit(m_location);
}

//-------------------------------------------------------------------
// This method begins an update of the viewer's view in a game world.
// A smooth transition path is prepared from information provided.
// The simplest update would be a single frame rendering from the
// viewer's current location. 
//-------------------------------------------------------------------

void Enigma::Viewer::update()
{	
	// Clear path variables.

	m_point.clear();

	// Set the initial translation path point.  The coarse location
	// values are set to the current viewer location, while the fine
	// location values begin at zero.
	
	m_point.m_east     = m_location.m_east;
	m_point.m_above    = m_location.m_above;
	m_point.m_north    = m_location.m_north;
	m_point.m_surface  = m_location.m_surface;
	m_point.m_rotation = m_location.m_rotation;
	
	// Save the current time before requesting a rendering.
	
	m_old_time = g_get_monotonic_time();
	m_signal_queue_render.emit();
}

//-------------------------------------------------------------------
// This method is called within an active OpenGL context requesting
// the viewer to render its view within the game world.  A transition
// path point will be advanced if a transition is in progress.
//-------------------------------------------------------------------
// context: Rendering GLContext.
// RETURN:  TRUE to stop other handlers from being invoked.
//-------------------------------------------------------------------

bool Enigma::Viewer::render(const Glib::RefPtr<Gdk::GLContext>& context)
{
	// Clear the view with a background colour.

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(INDOOR_R, INDOOR_G, INDOOR_B, -1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	bool repeat = false;
	bool done   = false;
	
	// Get the time that has elapsed since the last render request was sent.
	// The ratio of this elapsed time over the transition duration corresponds
	// to the ratio of the transition variable change (delta) over the total
	// transition change.
	
	gint64 new_time = g_get_monotonic_time();
	float elapsed   = new_time - m_old_time;
	m_old_time      = new_time;
	
	float delta;
	float remains;
	
	// Handle translation along East world axis.
	
	if (m_translate_east != 0)
	{
		delta   = (elapsed / TRANSITION_DURATION) * m_translate_east;
		remains = (float)m_translate_east - m_point.m_fine_east;
		
		if  (((m_translate_east > 0) && (remains > delta))
		  || ((m_translate_east < 0) && (remains < delta)))
		
		{
			m_point.m_fine_east += delta;
		}
		else
		{
			m_translate_east = 0;
			done = true;
		}
		
		repeat = true;
	}

	// Handle translation along Above world axis.

	if (m_translate_above != 0)
	{
		delta   = (elapsed / TRANSITION_DURATION) * m_translate_above;
		remains = (float)m_translate_above - m_point.m_fine_above;
		
		if  (((m_translate_above > 0) && (remains > delta))
		  || ((m_translate_above < 0) && (remains < delta)))
		
		{
			m_point.m_fine_above += delta;
		}
		else
		{
			m_translate_above = 0;
			done = true;
		}
		
		repeat = true;
	}

	// Handle translation along North world axis.

	if (m_translate_north != 0)
	{
		delta   = (elapsed / TRANSITION_DURATION) * m_translate_north;
		remains = (float)m_translate_north - m_point.m_fine_north;
		
		if  (((m_translate_north > 0) && (remains > delta))
		  || ((m_translate_north < 0) && (remains < delta)))
		
		{
			m_point.m_fine_north += delta;
		}
		else
		{
			m_translate_north = 0;
			done = true;
		}
		
		repeat = true;
	}

	// Handle rotation around East world axis.

	if (m_rotate_east != 0)
	{
		delta   = (elapsed / TRANSITION_DURATION) * m_rotate_east;
		remains = (float)m_rotate_east - m_point.m_fine_rotate_east;
		
		if  (((m_rotate_east > 0) && (remains > delta))
		  || ((m_rotate_east < 0) && (remains < delta)))
		
		{
			m_point.m_fine_rotate_east += delta;
		}
		else
		{
			m_rotate_east = 0;
			done = true;
		}
		
		repeat = true;
	}

	// Handle rotation around Above world axis.

	if (m_rotate_above != 0)
	{
		delta   = (elapsed / TRANSITION_DURATION) * m_rotate_above;
		remains = (float)m_rotate_above - m_point.m_fine_rotate_above;
		
		if  (((m_rotate_above > 0) && (remains > delta))
		  || ((m_rotate_above < 0) && (remains < delta)))
		
		{
			m_point.m_fine_rotate_above += delta;
		}
		else
		{
			m_rotate_above = 0;
			done = true;
		}
		
		repeat = true;
	}
	
	// Handle rotation around North world axis.
	
	if (m_rotate_north != 0)
	{
		delta   = (elapsed / TRANSITION_DURATION) * m_rotate_north;
		remains = (float)m_rotate_north - m_point.m_fine_rotate_north;
		
		if  (((m_rotate_north > 0) && (remains > delta))
		  || ((m_rotate_north < 0) && (remains < delta)))
		
		{
			m_point.m_fine_rotate_north += delta;
		}
		else
		{
			m_rotate_north = 0;
			done = true;
		}
		
		repeat = true;
	}

	if (done)
	{
		// A transition is done.  Set the viewer to this new location,
		// and emit the location in a signal.
		
		m_location = m_next_location;
		m_object->m_location = m_location;
		m_signal_location.emit(m_location);
		
		// Set the viewer point to the new location, then render the view
		// from this location.
		
		m_point.set(m_location);
		render_viewcone();
	
		// Call the "update done" slot to indicate the transition is done.
	
		m_slot_update_done();
	}
	else
		render_viewcone();

	// Request another rendering if a transition is not yet done.

	if (repeat)
		m_signal_queue_render.emit();

	return true;
}

//---------------------------------------------------------------------
// This method sweeps over a volume of positions to locate ones visible
// by the viewer.  Objects in a visible position are placed in buffers
// for rendering.
//---------------------------------------------------------------------

void Enigma::Viewer::render_viewcone()
{
	// Clear object buffers.
	
	m_objects.clear();
	m_items.clear();

	// Apply coarse rotation to viewcone.  This establishes the relative
	// rotation origin for additional fine rotations.
	
	Enigma::Matrix4 viewcone = m_viewcone;
	
	switch (m_location.m_rotation)
	{			
		case Enigma::Location::Direction::SOUTH:
			viewcone.rotate_y(180.0f);
			break;
			
		case Enigma::Location::Direction::EAST:
			viewcone.rotate_y(-90.0f);
			break;
			
		case Enigma::Location::Direction::WEST:
			viewcone.rotate_y(90.0f);
			break;
			
		case Enigma::Location::Direction::ABOVE:
			viewcone.rotate_x(90.0f);
			break;
			
		case Enigma::Location::Direction::BELOW:
			viewcone.rotate_x(-90.0f);
			break;
			
		default:
			break;
	}
	
	// Apply fine rotations to viewcone.
	
	GLfloat angle_x = m_point.m_fine_rotate_east;
	GLfloat angle_y = m_point.m_fine_rotate_above;
	GLfloat angle_z = m_point.m_fine_rotate_north;
	
	if ((angle_x > 0.0f) || (angle_x < 0.0f))
		viewcone.rotate_x(angle_x);
	else if ((angle_y > 0.0f) || (angle_y < 0.0f))
		viewcone.rotate_y(angle_y);
	else if ((angle_z > 0.0f) || (angle_z < 0.0f))
		viewcone.rotate_z(angle_z);
	
	// Extract left clipping plane coefficients and normalize them.
	
	GLfloat* array = viewcone.array();
	
	GLfloat left_a = array[12] + array[0];
	GLfloat left_b = array[13] + array[1];
	GLfloat left_c = array[14] + array[2];
	GLfloat left_d = array[15] + array[3];

	normalize_plane(left_a, left_b, left_c, left_d);

	// Extract right clipping plane coefficients and normalize them.

	GLfloat right_a = array[12] - array[0];
	GLfloat right_b = array[13] - array[1];
	GLfloat right_c = array[14] - array[2];
	GLfloat right_d = array[15] - array[3];

	normalize_plane(right_a, right_b, right_c, right_d);

	// Extract top clipping plane coefficients and normalize them.
	
	GLfloat top_a = array[12] - array[4];
	GLfloat top_b = array[13] - array[5];
	GLfloat top_c = array[14] - array[6];
	GLfloat top_d = array[15] - array[7];

	normalize_plane(top_a, top_b, top_c, top_d);

	// Extract bottom clipping plane coefficients and normalize them.

	GLfloat bottom_a = array[12] + array[4];
	GLfloat bottom_b = array[13] + array[5];
	GLfloat bottom_c = array[14] + array[6];
	GLfloat bottom_d = array[15] + array[7];

	normalize_plane(bottom_a, bottom_b, bottom_c, bottom_d);

	// Extract near clipping plane coefficients and normalize them.

	GLfloat near_a = array[12] + array[8];
	GLfloat near_b = array[13] + array[9];
	GLfloat near_c = array[14] + array[10];
	GLfloat near_d = array[15] + array[11];

	normalize_plane(near_a, near_b, near_c, near_d);
	
	// Derive a fine-resolution world-axis-aligned boundary of the viewcone
	// frustrum, relative to the viewer's position.  This boundary is set
	// by the eight corners of the frustrum.

	m_box_low_x  = 0.0f;
	m_box_high_x = 0.0f;
	m_box_low_y  = 0.0f;
	m_box_high_y = 0.0f;
	m_box_low_z  = 0.0f;
	m_box_high_z = 0.0f;

	adjust_boundary(left_a, left_b, left_c, top_a, top_b, top_c);
	adjust_boundary(left_a, left_b, left_c, -bottom_a, -bottom_b, bottom_c);
	adjust_boundary(-right_a, -right_b, -right_c, top_a, top_b, top_c);
	adjust_boundary(right_a, right_b, right_c, bottom_a, bottom_b, bottom_c);
	
	// Translate the viewcone boundary using the fine viewer offsets,
	// ensuring the viewcone boundary moves with the viewer.  The scale
	// difference between X,Y,Z and East,Above,North is taken into account
	// with the FULL_ROOM multiplier.
	
	m_box_low_x  += m_point.m_fine_east * FULL_ROOM;
	m_box_high_x += m_point.m_fine_east * FULL_ROOM;
	m_box_low_y  += m_point.m_fine_above * FULL_ROOM;
	m_box_high_y += m_point.m_fine_above * FULL_ROOM;
	m_box_low_z  -= m_point.m_fine_north * FULL_ROOM;
	m_box_high_z -= m_point.m_fine_north * FULL_ROOM;

	// Derive a coarse-resolution world-axis-aligned boundary of viewcone
	// frustrum, relative to the viewer's position.  A rounding-up constant
	// is added to ensure a change occurs before the viewer moves outside
	// the current limit.  The sign difference between North and Z is taken
	// into account with a change of sign and assignment to the opposite
	// position limit.
	
	short box_low_east   = (short)((m_box_low_x - ROUND_UP) / FULL_ROOM);
	short box_high_east  = (short)((m_box_high_x + ROUND_UP) / FULL_ROOM);
	short box_low_above  = (short)((m_box_low_y - ROUND_UP) / FULL_ROOM);
	short box_high_above = (short)((m_box_high_y + ROUND_UP) / FULL_ROOM);
	short box_high_north = (short)-((m_box_low_z - ROUND_UP) / FULL_ROOM);
	short box_low_north  = (short)-((m_box_high_z + ROUND_UP) / FULL_ROOM);

	// Adjust these limits so they will not cross the world boundary when
	// added to the viewer position.  This adjustment eliminates the need
	// for a boundary test while iterating over the positions later on.
	
	int gap = (int)m_world_boundary->m_upper_east - m_location.m_east;
	
	if (gap < box_high_east)
		box_high_east = (short)gap;
	
	gap = (int)m_world_boundary->m_lower_east - m_location.m_east;
		
	if (gap > box_low_east)
		box_low_east = (short)gap;
	
	gap = (int)m_world_boundary->m_upper_above - m_location.m_above;
	
	if (gap < box_high_above)
		box_high_above = (short)gap;
	
	gap = (int)m_world_boundary->m_lower_above - m_location.m_above;
	
	if (gap > box_low_above)
		box_low_above = (short)gap;
	
	gap = (int)m_world_boundary->m_upper_north - m_location.m_north;
	
	if (gap < box_high_north)
		box_high_north = (short)gap;
	
	gap = (int)m_world_boundary->m_lower_north - m_location.m_north;
	
	if (gap > box_low_north)
		box_low_north = (short)gap;

	// Calculate the initial limits of the fine room center positions.
	// These include the offset from the camera at the viewcone apex
	// to the viewer at the near clipping plane, and the current fine
	// translation of the viewer.
	
	GLfloat low_x = box_low_east * FULL_ROOM
	              + near_a * NEAR_CLIP
	              - m_point.m_fine_east * FULL_ROOM;

	GLfloat low_y = box_low_above * FULL_ROOM
	              + near_b * NEAR_CLIP
	              - m_point.m_fine_above * FULL_ROOM;
	              
	GLfloat high_z = -box_low_north * FULL_ROOM
	               + near_c * NEAR_CLIP
	               + m_point.m_fine_north * FULL_ROOM;
	
	// Iterate over all world positions contained within the boundary of
	// the viewcone, culling all positions that are not visible (do not
	// cross into the positive inner space of the viewcone frustrum),
	// and adding objects in visible positions to a list for rendering. 
	
	short east, above, north;
	GLfloat x, y, z;
	Enigma::Location position;

	east = box_low_east;
	x    = low_x;
	
	while (east <= box_high_east)
	{
		above = box_low_above;
		y     = low_y;
		
		while (above <= box_high_above)
		{
			north = box_low_north;
			z     = high_z;
			
			while (north <= box_high_north)
			{
				// Test the visibility of the position using a spherical boundary.
				// The position is visible if it lies at a positive signed distance
				// from all clipping planes (lies in positive half-space).  A radius
				// test is used instead of a far clipping plane.
				
				if ((((left_a * x) + (left_b * y) + (left_c * z) + left_d
				     + BOUNDING_RADIUS) > 0)
				  && (((right_a * x) + (right_b * y) + (right_c * z) + right_d
				     + BOUNDING_RADIUS) > 0)
				  && (((bottom_a * x) + (bottom_b * y) + (bottom_c * z) + bottom_d
				     + BOUNDING_RADIUS) > 0)
				  && (((top_a * x) + (top_b * y) + (top_c * z) + top_d
				     + BOUNDING_RADIUS) > 0)
				  && ((x*x + y*y + z*z) <= FAR_RADIUS_SQUARED))
				{
					// The position is visible within the viewcone.
					// Calculate its absolute position in the world.
					
					position.m_east  = m_location.m_east + east;
					position.m_above = m_location.m_above + above;
					position.m_north = m_location.m_north + north;
					
					// Find objects in the position.
			
					m_world_objects->find(position, m_objects);
					m_world_items->find(position, m_items);
				}
				
				// Calculate the next coarse and fine North room center position.
				// The sign difference between North and Z is taken into account
				// by decreasing Z in the opposite direction.

				north += 1;
				z     -= FULL_ROOM;
			}

			// Calculate the next coarse and fine Above room center position.
		
			above += 1;
			y     += FULL_ROOM;
		}
		
		// Calculate the next coarse and fine East room center position.
		
		east += 1;
		x    += FULL_ROOM;
	}
	
	// Render all objects found in the viewcone.
	
	std::shared_ptr<Enigma::Object> object;
	std::shared_ptr<Enigma::Item> item;
	int total;
	int index;
	
	total = m_objects.size();
	
	for (index = 0; index < total; index ++)
	{
		object = m_objects.at(index);	
		object->render(m_point);
	}
	
	total = m_items.size();
	
	for (index = 0; index < total; index ++)
	{
		item = m_items.at(index);	
		item->render(m_point);
	}	
}

//--------------------------------------------------------------
// This method normalizes the coefficients of a plane.
//--------------------------------------------------------------
// a, b, c, d: Reference to plane coefficients to be normalized.
//--------------------------------------------------------------

void Enigma::Viewer::normalize_plane(
	GLfloat& a, GLfloat& b, GLfloat& c, GLfloat& d)
{
	GLfloat magnitude = sqrt(a*a + b*b + c*c);
	
	a = a / magnitude;
	b = b / magnitude;
	c = c / magnitude;
	d = d / magnitude;
}

//---------------------------------------------------------------------
// This private function adjusts the viewcone boundary from unit normal
// vectors of two planes intersecting at right angles.
//---------------------------------------------------------------------
// i0, j0, k0: Unit normal vector for plane 0.
// i1, j1, k1: Unit normal vector for plane 1.
//---------------------------------------------------------------------

void Enigma::Viewer::adjust_boundary(
	GLfloat i0, GLfloat j0, GLfloat k0, GLfloat i1, GLfloat j1, GLfloat k1)
{
	// Take the cross product of the unit normal vectors of the two planes.
	// Since the planes intersect at right angles, the resulting vector
	// pointing along the line of intersection will also be a unit vector.

	GLfloat i = (j0 * k1) - (k0 * j1);
	GLfloat j = (k0 * i1) - (i0 * k1);
	GLfloat k = (i0 * j1) - (j0 * i1);

	// Adjust boundary to include points on near and far clipping plane.

	adjust_boundary(m_near_edge, i, j, k);
	adjust_boundary(m_far_edge, i, j, k);
}

//--------------------------------------------------------------------
// This method adjusts the viewcone boundary from a magnitude and unit
// vector pointing along the intersection of two clipping planes.
//--------------------------------------------------------------------
// magnitude: Magnitude of vector.
// i, j, k:   Unit vector of direction.
//--------------------------------------------------------------------

void Enigma::Viewer::adjust_boundary(
	GLfloat magnitude, GLfloat i, GLfloat j, GLfloat k)
{
	// Adjust the boundary as needed.
	
	adjust_boundary(magnitude * i, m_box_low_x, m_box_high_x);
	adjust_boundary(magnitude * j, m_box_low_y, m_box_high_y);
	adjust_boundary(magnitude * k, m_box_low_z, m_box_high_z);
}

//----------------------------------------------------------
// This method adjusts the viewcone boundary from an offset.
//----------------------------------------------------------
// offset:    Boundary offset.
// low, high: Reference to boundary limits to be adjusted.
//----------------------------------------------------------

void Enigma::Viewer::adjust_boundary(
	GLfloat offset, GLfloat& low, GLfloat& high)
{
	if (offset < low)
		low = offset;
	else if (offset > high)
		high = offset;
}

//---------------------------------------------------------------
// This method emits the current location in a "location" signal.
//---------------------------------------------------------------

void Enigma::Viewer::do_location()
{
	m_signal_location.emit(m_location);
}

//--------------------------------------------------
// This method returns the "location" signal server.
//--------------------------------------------------

Enigma::Viewer::type_signal_location Enigma::Viewer::signal_location()
{
	return m_signal_location;
}

//------------------------------------------------------
// This method returns the "queue_render" signal server.
//------------------------------------------------------

Enigma::Viewer::type_signal_queue_render Enigma::Viewer::signal_queue_render()
{
	return m_signal_queue_render;
}

//-----------------------------------------------------------
// This method returns a reference to the "update done" slot.
//-----------------------------------------------------------

Enigma::Viewer::type_slot_update_done& Enigma::Viewer::slot_update_done()
{
	return m_slot_update_done;
}
