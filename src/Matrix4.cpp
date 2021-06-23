// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Matrix4 class implementation.  The Matrix4 class describes
// a 4x4 element OpenGL transformation matrix.
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

#include <stdio.h>
#include <limits>
#include <math.h>
#include "Matrix4.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::Matrix4::Matrix4()
{
	// Zero all matrix elements.
	
	zero();
}

//-----------------------------------------------------------
// This method returns a pointer to the matrix element array.
//-----------------------------------------------------------

GLfloat* Enigma::Matrix4::array()
{
	return m_array;
}

//----------------------------------------------------------------------
// This method sets the matrix elements equal to that of another matrix.
//----------------------------------------------------------------------
// matrix: Matrix to be equated.
//----------------------------------------------------------------------

void Enigma::Matrix4::operator =(Enigma::Matrix4& matrix)
{
	// Copy other matrix elements to matrix.
	
	GLfloat* array = matrix.array();
	
	for (int i = 0; i < Matrix4::TOTAL; i++)
		m_array[i] = array[i];
}

//----------------------------------------------
// This method sets all matrix elements to zero.
//----------------------------------------------

void Enigma::Matrix4::zero()
{
	// Zero all matrix elements.
	
	for (int i = 0; i < Matrix4::TOTAL; i++)
		m_array[i] = 0.0f;
}

//--------------------------------------------------
// This method sets the matrix to its identity form.
//--------------------------------------------------

void Enigma::Matrix4::identity()
{
	// Set unity scale factor in matrix.
	
	set_scale(1.0f);
}

//-----------------------------------------------
// This method sets a scale factor in the matrix.
//-----------------------------------------------
// factor: Scale factor.
//-----------------------------------------------

void Enigma::Matrix4::set_scale(GLfloat factor)
{	
	// Zero all matrix elements.
	
	zero();
	
	// Populate diagonal elements with scale factor.
	
	m_array[0]  = factor;
	m_array[5]  = factor;
	m_array[10] = factor;
	m_array[15] = 1.0f;
}

//--------------------------------------------------
// This method applies a scale factor to the matrix.
//--------------------------------------------------
// factor: Scale factor.
//--------------------------------------------------

void Enigma::Matrix4::scale(GLfloat factor)
{
	// Prepare scaling matrix.
	
	Enigma::Matrix4 matrix;
	matrix.set_scale(factor);
	
	// Apply scale factor to matrix.
	
	multiply(matrix);
}

//----------------------------------------------
// This method sets a translation in the matrix.
//----------------------------------------------
// dx: Change to make to x coordinate element.
// dy: Change to make to y coordinate element.
// dz: Change to make to z coordinate element.
//----------------------------------------------

void Enigma::Matrix4::set_translate(GLfloat dx, GLfloat dy, GLfloat dz)
{
	// Set matrix to the identity form.
	
	identity();
	
	// Populate elements with translation.

	m_array[12] = dx;
	m_array[13] = dy;
	m_array[14] = dz;
}

//-------------------------------------------------
// This method applies a translation to the matrix.
//-------------------------------------------------
// dx: Change to make to x coordinate element.
// dy: Change to make to y coordinate element.
// dz: Change to make to z coordinate element.
//-------------------------------------------------

void Enigma::Matrix4::translate(GLfloat dx, GLfloat dy, GLfloat dz)
{
	// Prepare translation matrix.
	
	Enigma::Matrix4 matrix;
	matrix.set_translate(dx, dy, dz);
	
	// Apply translation to matrix.
	
	multiply(matrix);
}

//---------------------------------------------------
// This method sets an x-axis rotation in the matrix.
//---------------------------------------------------
// angle: Degrees of rotation.
//---------------------------------------------------

void Enigma::Matrix4::set_rotate_x(GLfloat angle)
{
	GLfloat radians = (2.0f * Matrix4::PI * angle) / 360.0f;
	GLfloat cos_a = cos(radians);
	GLfloat sin_a = sin(radians);
	
	// Zero all matrix elements.
	
	zero();
	
	// Populate x-rotation and unity matrix elements.
	
	m_array[0] = 1.0f;
	m_array[5] = cos_a;
	m_array[6] = sin_a;
	m_array[9] = -sin_a;
	m_array[10] = cos_a;
	m_array[15] = 1.0f;
}

//------------------------------------------------------
// This method applies an x-axis rotation to the matrix.
//------------------------------------------------------
// angle: Degrees of rotation.
//------------------------------------------------------

void Enigma::Matrix4::rotate_x(GLfloat angle)
{
	// Prepare rotation matrix.
	
	Enigma::Matrix4 matrix;
	matrix.set_rotate_x(angle);
	
	// Apply rotation to matrix.
	
	multiply(matrix);
}

//--------------------------------------------------
// This method sets a y-axis rotation in the matrix.
//--------------------------------------------------
// angle: Degrees of rotation.
//--------------------------------------------------

void Enigma::Matrix4::set_rotate_y(GLfloat angle)
{
	GLfloat radians = (2.0f * Matrix4::PI * angle) / 360.0f;
	GLfloat cos_a = cos(radians);
	GLfloat sin_a = sin(radians);
	
	// Zero all matrix elements.
	
	zero();
	
	// Populate y-rotation and unity matrix elements.
	
	m_array[0] = cos_a;
	m_array[2] = -sin_a;
	m_array[5] = 1.0f;
	m_array[8] = sin_a;
	m_array[10] = cos_a;
	m_array[15] = 1.0f;
}

//-----------------------------------------------------
// This method applies a y-axis rotation to the matrix.
//-----------------------------------------------------
// angle: Degrees of rotation.
//-----------------------------------------------------

void Enigma::Matrix4::rotate_y(GLfloat angle)
{
	// Prepare rotation matrix.
	
	Enigma::Matrix4 matrix;
	matrix.set_rotate_y(angle);
	
	// Apply rotation to matrix.
	
	multiply(matrix);
}

//--------------------------------------------------
// This method sets a z-axis rotation in the matrix.
//--------------------------------------------------
// angle: Degrees of rotation.
//--------------------------------------------------

void Enigma::Matrix4::set_rotate_z(GLfloat angle)
{
	GLfloat radians = (2.0f * Matrix4::PI * angle) / 360.0f;
	GLfloat cos_a = cos(radians);
	GLfloat sin_a = sin(radians);
	
	// Zero all matrix elements.
	
	zero();
	
	// Populate z-rotation and unity matrix elements.
	
	m_array[0] = cos_a;
	m_array[1] = sin_a;
	m_array[4] = -sin_a;
	m_array[5] = cos_a;
	m_array[10] = 1.0f;
	m_array[15] = 1.0f;
}

//-----------------------------------------------------
// This method applies a z-axis rotation to the matrix.
//-----------------------------------------------------
// angle: Degrees of rotation.
//-----------------------------------------------------

void Enigma::Matrix4::rotate_z(GLfloat angle)
{
	// Prepare rotation matrix.
	
	Enigma::Matrix4 matrix;
	matrix.set_rotate_z(angle);
	
	// Apply rotation to matrix.
	
	multiply(matrix);
}

//-----------------------------------------------------
// This method multiplies the matrix by another matrix.
//-----------------------------------------------------
// matrix: Other matrix.
//-----------------------------------------------------

void Enigma::Matrix4::multiply(Enigma::Matrix4 matrix)
{
	GLfloat* other_array = matrix.array();
	GLfloat product[Matrix4::TOTAL];
	int i, j, k;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			for (k = 0, product[i + j * 4] = 0.0f; k < 4; k++)
				product[i + j * 4] += other_array[i + k * 4] * m_array[k + j * 4];

	// Copy product elements to matrix.

	for (i = 0; i < Matrix4::TOTAL; i ++)
		m_array[i] = product[i];
}

//--------------------------------------------------
// This method sets a perspective projection matrix.
//--------------------------------------------------
// field_of_view: Field of view angle (degrees).
// aspect_ratio:  View aspect ratio.
// near, far:     Clipping plane distances.
//--------------------------------------------------

void Enigma::Matrix4::perspective(GLfloat field_of_view,
                                  GLfloat aspect_ratio,
                                  GLfloat near_clip, GLfloat far_clip)
{
	GLfloat height = near_clip * tan(field_of_view * (Matrix4::PI / 360.0f));
	GLfloat width  = height * aspect_ratio;

	frustrum(-width, width, -height, height, near_clip, far_clip);
}

//------------------------------------------------------------------
// This method sets the frustrum for a projection matrix.
//------------------------------------------------------------------
// left_clip, right_clip: Left and right clipping plane distances.
// top_clip, bottom_clip: Top and bottom clipping plane distances.
// near_clip, far_clip:   Near and far clipping plane distances.
//------------------------------------------------------------------

void Enigma::Matrix4::frustrum(GLfloat left_clip, GLfloat right_clip,
                               GLfloat bottom_clip, GLfloat top_clip,
                               GLfloat near_clip, GLfloat far_clip)
{
	GLfloat GLfloat_max = std::numeric_limits<GLfloat>::max();
	
	// Set constant elements.
	
	m_array[1]  = 0.0f;
	m_array[2]  = 0.0f;
	m_array[3]  = 0.0f;
	m_array[4]  = 0.0f;
	m_array[6]  = 0.0f;
	m_array[7]  = 0.0f;
	m_array[11] = -1.0f;
	m_array[12] = 0.0f;
	m_array[13] = 0.0f;
	m_array[15] = 0.0f;
	
	// Set elements based on left and right clipping plane coordinates.
	
	GLfloat delta;
	
	if (left_clip != right_clip)
	{
		delta   = right_clip - left_clip;
		m_array[0] = (2.0f * near_clip) / delta;
		m_array[8] = (right_clip + left_clip) / delta;
	}
	else
	{
		m_array[0] = GLfloat_max;
		m_array[8] = GLfloat_max;
	}
	
	// Set elements based on bottom and top clipping plane coordinates.
	
	if (bottom_clip != top_clip)
	{
		delta   = top_clip - bottom_clip;
		m_array[5] = (2.0f * near_clip) / delta;
		m_array[9] = (top_clip + bottom_clip) / delta;
	}
	else
	{
		m_array[5] = GLfloat_max;
		m_array[9] = GLfloat_max;
	}
	
	// Set elements based on near and far clipping plane coordinates.
	
	if (near_clip != far_clip)
	{
		//delta = far_clip - near_clip;
		//m_array[10] = -(far_clip + near_clip) / delta;
		//m_array[14] = (-2.0f * far_clip * near_clip) / delta;
		
		delta = far_clip - near_clip;
		m_array[10] = -far_clip / delta;
		m_array[14] = -(far_clip * near_clip) / delta;
	}
	else
	{
		m_array[10] = GLfloat_max;
		m_array[14] = GLfloat_max;
	}
}
