// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Mesh class implementation.  The Mesh class manages
// OpenGL image mesh data for a world object.
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

#include <iostream>
#include <glibmm/refptr.h>
#include <glibmm/bytes.h>
#include <giomm/resource.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include "Mesh.h"
#include "Matrix4.h"

//-------------------
// Local declarations
//-------------------

static const std::string vertex_shader_code   = "VertexShader.glsl";
static const std::string fragment_shader_code = "FragmentShader.glsl";

// Vertex shader attribute and uniform locations.  These will be initialized
// after the shader program has been compiled and linked.

static GLint vPosition;
static GLint vColour;
static GLint mTransform;

// Perspective projection values.

static const GLfloat NEAR_CLIP     = 1.0f;
static const GLfloat FAR_CLIP      = 10.0f;
static const GLfloat FIELD_OF_VIEW = 60.0f;
static const GLfloat ASPECT_RATIO  = 1.0f;

static const GLfloat CAMERA_OFFSET = -1.15f;  // Camera offset.
static const GLfloat FULL_ROOM     = 2.0f;    // Width of a world room.
static const GLfloat HALF_ROOM     = 1.0f;    // Room center to outer walls.

//-------------------------------------------------
// This constructor creates an empty Mesh instance.
//-------------------------------------------------

Enigma::Mesh::Mesh()
{
	// Assign buffer objects a "name" of zero to indicate they are not yet
	// associated with any object.
	
	m_position_bo = 0;
	m_colour_bo   = 0;
}

//---------------------------------------------------------------------------
// This method loads vertex data from resources and uploads it to server-side
// buffer objects.
//---------------------------------------------------------------------------
// name: Mesh resource name (eg. "BlockWall.msh")
//---------------------------------------------------------------------------

void Enigma::Mesh::load(std::string name)
{
	// Extract the mesh data from the resource bundle.  Multi-byte values
	// in this bundle are in little-endian format.

	std::string path = G_RESOURCE_PREFIX;
	path += name;
	
	Glib::RefPtr<const Glib::Bytes> byte_array =
		Gio::Resource::lookup_data_global(path, Gio::RESOURCE_LOOKUP_FLAGS_NONE);

	gsize size;
	guint8* byte = (guint8*)byte_array->get_data(size);
	
	// Return immediately if there is too little remaining data,
	// or the header ID ("mesh\n") is wrong.

	if (size < 4)
		return;
	
	if  ((*(byte + 0) != 'm')
		|| (*(byte + 1) != 's')
		|| (*(byte + 2) != 'h')
		|| (*(byte + 3) != '\n'))
	{	
		return;
	}
	
	// Skip over header ID to the vertex total (4 bytes).
	// Return immediately if there is too little remaining data.
	
	byte += 4;
	size -= 4;
	
	if (size < 4)
		return;
	
	int vertex_total = (*(byte + 0) 
	                 + (*(byte + 1) << 8)
	                 + (*(byte + 2) << 16)
	                 + (*(byte + 3) << 24));
	
	// Skip over the vertex total to the vertex data (array of 4-byte values).
	// Return immediately if there is too little remaining data.  Each vertex
	// coordinate uses 12 bytes (x,y,z 3-tuples), and each vertex colour uses
	// 16 bytes (r,g,b,a 4-tuples).

	byte += 4;
	size -= 4;
		
	if (size < vertex_total * (12 + 16))
		return;
	
	// Declare a structure to convert from GLuint and GLfloat types.
	
	union
	{
		GLuint gl_integer;
		GLfloat gl_float;
	} convert;

	// Stream resource data into the vertex position buffer.
	
	std::vector<GLfloat> position;
	position.reserve(vertex_total * 3);
		
	int total = vertex_total * 3;
	
	while (total > 0)
	{
		// Extract a vertex position stored as a 4-byte OpenGL integer
		// value, and convert it into an OpenGL floating-point value.
		
		convert.gl_integer = (*(byte + 0) 
                       + (*(byte + 1) << 8)
                       + (*(byte + 2) << 16)
                       + (*(byte + 3) << 24));
		
		position.emplace_back(convert.gl_float);
		
		total -= 1;
		byte  += 4;
	}
	
	// Stream resource data into the vertex colour buffers.
	
	std::vector<GLfloat> colour;
	colour.reserve(vertex_total * 4);
	
	total = vertex_total * 4;
	
	while (total > 0)
	{
		// Extract a vertex colour stored as a 4-byte OpenGL integer
		// value, and convert it to an OpenGL floating-point value.
		
		convert.gl_integer = (*(byte + 0) 
                       + (*(byte + 1) << 8)
                       + (*(byte + 2) << 16)
                       + (*(byte + 3) << 24));
                      
		colour.emplace_back(convert.gl_float);
		
		total -= 1;
		byte  += 4;
	}
	
	// Create server-side buffer object to contain vertex positions,
	// and copy the client-side data to this buffer. 

	glGenBuffers(1, &m_position_bo);
	glBindBuffer(GL_ARRAY_BUFFER, m_position_bo);
	
	glBufferData(GL_ARRAY_BUFFER,
	             position.size() * sizeof(GL_FLOAT),
	             position.data(),
	             GL_STATIC_DRAW);

	// Create server-side buffer object to contain vertex colours,
	// and copy the client-side data to this buffer.  
	
	glGenBuffers(1, &m_colour_bo);
	glBindBuffer(GL_ARRAY_BUFFER, m_colour_bo);
	
	glBufferData(GL_ARRAY_BUFFER,
	             colour.size() * sizeof(GL_FLOAT),
	             colour.data(),
	             GL_STATIC_DRAW);
}

//--------------------------------------------------------------
// This method renders the mesh within an active OpenGL context.
//--------------------------------------------------------------
// object: Location of object in the world.
// viewer: Location of viewer in the world.
//--------------------------------------------------------------

void Enigma::Mesh::render(const Enigma::Location& object,
			                    const Enigma::FineLocation& viewer)
{
	// Prepare a transformation identity matrix for the object.
	
	Enigma::Matrix4 matrix;
	matrix.identity();

	// The initial "raw" mesh lies on the model space x-y plane, rising up
	// from the positive z-axis from around z = 0.  This corresponds to
	// an object with a rotation of Above in the center of a world room
	// at the origin (East = 0, Above = 0, North = 0).  Apply coarse rotations
	// and translations to properly orient this mesh within the origin room.

	Location::Direction surface  = object.m_surface;
	Location::Direction rotation = object.m_rotation;

	switch (surface)
	{
		case Location::Direction::NORTH:
			// The mesh is on the North surface.
			// Rotate the mesh around the z-axis so it has
			// the correct rotation.

			if (rotation == Location::Direction::WEST)
				matrix.rotate_z(90.0f);
			else if (rotation == Location::Direction::BELOW)
				matrix.rotate_z(180.0f);
			else if (rotation == Location::Direction::EAST)
				matrix.rotate_z(-90.0f);

			// Move mesh to the North surface.

			matrix.translate(0, 0, -HALF_ROOM);
			break;
			
		case Location::Direction::SOUTH:
			// The mesh is on the South surface.
			
			matrix.rotate_y(180.0f);
			
			// Rotate the mesh around the z-axis so it has
			// the correct rotation.

			if (rotation == Location::Direction::EAST)
				matrix.rotate_z(-90.0f);
			else if (rotation == Location::Direction::BELOW)
				matrix.rotate_z(180.0f);
			else if (rotation == Location::Direction::WEST)
				matrix.rotate_z(90.0f);
			
			// Move mesh to the South surface.

			matrix.translate(0, 0, HALF_ROOM);
			break;

		case Location::Direction::EAST:
			// The mesh is on the East surface.
			
			matrix.rotate_y(-90.0f);
			
			// Rotate the mesh around the view x-axis so it has
			// the correct rotation.
			
			if (rotation == Location::Direction::SOUTH)
				matrix.rotate_x(90.0f);
			else if (rotation == Location::Direction::BELOW)
				matrix.rotate_x(180.0f);
			else if (rotation == Location::Direction::NORTH)
				matrix.rotate_x(-90.0f);
				
			// Move mesh to the East surface.
			
			matrix.translate(HALF_ROOM, 0, 0);
			break;

		case Location::Direction::WEST:
			// The mesh is on the West surface.
			
			matrix.rotate_y(90.0f);
			
			// Rotate the mesh around the view x-axis so it has
			// the correct rotation.
			
			if (rotation == Location::Direction::SOUTH)
				matrix.rotate_x(90.0f);
			else if (rotation == Location::Direction::BELOW)
				matrix.rotate_x(180.0f);
			else if (rotation == Location::Direction::NORTH)
				matrix.rotate_x(-90.0f);

			// Move mesh to the West surface.
			
			matrix.translate(-HALF_ROOM, 0, 0);
			break;
			
		case Location::Direction::ABOVE:
			// The object is on the Above surface.
		
			matrix.rotate_x(90.0f);
		
			// Rotate the mesh around the view y-axis so it has
			// the correct rotation.  
			
			if (rotation == Location::Direction::WEST)
				matrix.rotate_y(-90.0f);
			else if (rotation == Location::Direction::SOUTH)
				matrix.rotate_y(180.0f);
			else if (rotation == Location::Direction::EAST)
				matrix.rotate_y(90.0f);

			// Move mesh to the Above surface.

			matrix.translate(0, HALF_ROOM, 0);
			break;
			
		case Location::Direction::BELOW:
			// The mesh is on the Below surface.
		
			matrix.rotate_x(-90.0f);
			
			// Rotate the mesh around the view y-axis so it has
			// the correct rotation.  
			
			if (rotation == Location::Direction::WEST)
				matrix.rotate_y(90.0f);
			else if (rotation == Location::Direction::SOUTH)
				matrix.rotate_y(180.0f);
			else if (rotation == Location::Direction::EAST)
				matrix.rotate_y(-90.0f);

			// Move mesh to the Below surface.
			
			matrix.translate(0, -HALF_ROOM, 0);
			break;
			
		default:
			break;
	}

	// The viewer initially begins in the center of the origin world room
	// on the Below surface facing North.  Apply translations based on
	// the relative distance between the object and viewer.  These are based
	// on a unit room size of 1, so they must be scaled according.
	
	GLfloat translate_x = ((GLfloat)(object.m_east - viewer.m_east)
	                    - viewer.m_fine_east) * FULL_ROOM;
	
	GLfloat translate_y = ((GLfloat)(object.m_above - viewer.m_above)
	                    - viewer.m_fine_above) * FULL_ROOM;

	GLfloat translate_z = ((GLfloat)(object.m_north - viewer.m_north)
	                    - viewer.m_fine_north) * -FULL_ROOM;

	matrix.translate(translate_x, translate_y, translate_z);
	
	// The viewer now becomes a pivot point around which the entire space
	// rotates as the viewer rotates.  Apply coarse rotations that depend
	// on the viewer's surface and rotation on the surface.

	surface  = viewer.m_surface;
	rotation = viewer.m_rotation;
	
	switch (surface)
	{
		case Location::Direction::NORTH:
			// The viewer is on the North surface.  Rotate the viewer
			// so it is facing in the correct direction.
			
			matrix.rotate_x(-90.0f);
			
			if (rotation == Location::Direction::WEST)
				matrix.rotate_y(-90.0f);
			else if (rotation == Location::Direction::BELOW)
				matrix.rotate_y(180.0f);
			else if (rotation == Location::Direction::EAST)
				matrix.rotate_y(90.0f);
			break;
		
		case Location::Direction::SOUTH:
			// The viewer is on the South surface.  Rotate the viewer
			// so it is facing in the correct direction.
			
			matrix.rotate_x(90.0f);
			
			if (rotation == Location::Direction::WEST)
				matrix.rotate_y(-90.0f);
			else if (rotation == Location::Direction::ABOVE)
				matrix.rotate_y(180.0f);
			else if (rotation == Location::Direction::EAST)
				matrix.rotate_y(90.0f);

			break;
		
		case Location::Direction::EAST:
			// The viewer is on the East surface.  Rotate the viewer
			// so it is facing in the correct direction.
			
			matrix.rotate_z(-90.0f);
			
			if (rotation == Location::Direction::ABOVE)
				matrix.rotate_y(90.0f);
			else if (rotation == Location::Direction::SOUTH)
				matrix.rotate_y(180.0f);
			else if (rotation == Location::Direction::BELOW)
				matrix.rotate_y(-90.0f);
			
			break;
		
		case Location::Direction::WEST:
			// The viewer is on the East surface.  Rotate the viewer
			// so it is facing in the correct direction.
			
			matrix.rotate_z(90.0f);
			
			if (rotation == Location::Direction::ABOVE)
				matrix.rotate_y(-90.0f);
			else if (rotation == Location::Direction::SOUTH)
				matrix.rotate_y(180.0f);
			else if (rotation == Location::Direction::BELOW)
				matrix.rotate_y(90.0f);
			
			break;
		
		case Location::Direction::ABOVE:
			// The viewer is on the Above surface.  Rotate the viewer
			// so it is facing in the correct direction.
			
			matrix.rotate_x(180.0f);
			
			if (rotation == Location::Direction::WEST)
				matrix.rotate_y(-90.0f);
			else if (rotation == Location::Direction::NORTH)
				matrix.rotate_y(180.0f);
			else if (rotation == Location::Direction::EAST)
				matrix.rotate_y(90.0f);
			break;
		
		default:
			// The viewer is on the Below surface, which is the initial surface
			// for the viewer.  Rotate the viewer so it is facing in the correct
			// direction.
			
			if (rotation == Location::Direction::WEST)
				matrix.rotate_y(-90.0f);
			else if (rotation == Location::Direction::SOUTH)
				matrix.rotate_y(180.0f);
			else if (rotation == Location::Direction::EAST)
				matrix.rotate_y(90.0f);

			break;
	}

	// Apply fine viewer rotations.  The result of these rotations is sensitive
	// to the order applied, so only the first non-zero one will be applied.
	// Testing for an angle of zero is done to prevent unnecessary matrix
	// operations.  The sign of the angle is changed since the view rotates in
	// a direction opposite that of the viewer.  

	GLfloat angle_x = viewer.m_fine_rotate_east;
	GLfloat angle_y = viewer.m_fine_rotate_above;
	GLfloat angle_z = viewer.m_fine_rotate_north;
	
	if ((angle_x > 0.0f) || (angle_x < 0.0f))
		matrix.rotate_x(-angle_x);
	else if ((angle_y > 0.0f) || (angle_y < 0.0f))
		matrix.rotate_y(-angle_y);
	else if ((angle_z > 0.0f) || (angle_z < 0.0f))
		matrix.rotate_z(-angle_z);
	
	// If a wall-type mesh were rendered in front now, it would extend
	// beyond the viewport boundary.  This is how a camera located at 
	// the same position as the viewer would visualize the scene.  Pull
	// the camera back so a wall-type mesh is well within the viewport
	// and has a margin around it. 
	
	matrix.translate(0.0f, 0.0f, CAMERA_OFFSET);
	
	// Pass the transformation matrix to the mTransform uniform.

	glUniformMatrix4fv(mTransform, 1, GL_FALSE, matrix.array());

	// Attach vertex position buffer to the vPosition vertex attribute.

	glBindBuffer(GL_ARRAY_BUFFER, m_position_bo);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
			
	// Calculate the total number of vertices to be rendered.
	
	GLint total;
	
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &total);
	total = total / (sizeof(GL_FLOAT) * 3);

	// Attach vertex colour buffer to the vColour vertex attribute.
	
	glBindBuffer(GL_ARRAY_BUFFER, m_colour_bo);
	glVertexAttribPointer(vColour, 4, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Draw the mesh.
	
	glDrawArrays(GL_TRIANGLES, 0, total);
}

//------------------------------------------------------------------
// This static method initializes the Mesh class within an active
// OpenGL context.  Common rendering resources that include a shader
// program and bound Vertex Array Object are created.
//------------------------------------------------------------------

void Enigma::Mesh::initialize()
{
	// Create a shader program.
	
	GLuint program = glCreateProgram();
	
	// Read vertex shader code from the resource bundle.
	
	Glib::RefPtr<const Glib::Bytes> byte_array;
	GLchar* code;
	gsize size;
	std::string path;
	
	path = G_RESOURCE_PREFIX;
	path += vertex_shader_code;
	
	byte_array =
		Gio::Resource::lookup_data_global(path, Gio::RESOURCE_LOOKUP_FLAGS_NONE);
	
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	code                 = (GLchar*)byte_array->get_data(size);
	
	glShaderSource(vertex_shader, 1, &code, NULL);
	
	// Compile vertex shader code and attach it to the shader program.
	
	glCompileShader(vertex_shader);
	glAttachShader(program, vertex_shader);
	
	// Read fragment shader code from the resource bundle.

	path = G_RESOURCE_PREFIX;
	path += fragment_shader_code;

	byte_array =
		Gio::Resource::lookup_data_global(path, Gio::RESOURCE_LOOKUP_FLAGS_NONE);
		                                  
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	code                   = (GLchar*)byte_array->get_data(size);
	
	glShaderSource(fragment_shader, 1, &code, NULL);
	
	// Compile vertex shader code and attach it to the shader program.
	
	glCompileShader(fragment_shader);
	glAttachShader(program, fragment_shader);

	// Link shader program containing the two compiled shaders, then check	
	// the program link status.  If linking was not successful, output
	// linking log as an error message.
	
	glLinkProgram(program);
	
	GLint status = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	
	if (status == GL_FALSE)
	{
		std::cerr << "Failed linking of shader program." << std::endl;
		
		GLint length;
		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);

		if (length > 0)
		{
			GLchar log[length];
			glGetProgramInfoLog(program, sizeof(log), NULL, log);
			std::cerr << log << std::endl;
		}
		else
			std::cerr << "No linking log available." << std::endl;
	}

	// The linked program is now ready to use.
	
	glUseProgram(program);
	
	// Record the shader attribute and uniform locations assigned by the linker.
	
	vPosition   = glGetAttribLocation(program, "vPosition");
	vColour     = glGetAttribLocation(program, "vColour");
	mTransform  = glGetUniformLocation(program, "mTransform");
	
	// Unbind and delete the shaders since they are no longer needed
	// once the program has been linked.
	
	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
		
	// Create and bind a common Vertex Array Object to use when rendering
	// all Mesh objects.
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	// Enable all vertex attributes.
	
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vColour);
	
	// Set a perspective projection matrix.  This remains constant
	// for all rendering.

	GLint mProjection = glGetUniformLocation(program, "mProjection");	
	
	Enigma::Matrix4 matrix;
	matrix.perspective(FIELD_OF_VIEW, ASPECT_RATIO,
	                   NEAR_CLIP, FAR_CLIP);

	glUniformMatrix4fv(mProjection, 1, GL_FALSE, matrix.array());
}

