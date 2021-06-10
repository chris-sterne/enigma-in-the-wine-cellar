// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Matrix4 class header.  The Matrix4 class describes
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

#ifndef __ENIGMA_MATRIX4_H__
#define __ENIGMA_MATRIX4_H__

#include <GL/gl.h>

namespace Enigma
{
	class Matrix4
	{
		public:
		  // Public methods.

		  Matrix4();
		  void zero();
		  void identity();
		  void set_scale(GLfloat factor);
		  void scale(GLfloat factor);
			void set_rotate_x(GLfloat angle);
			void rotate_x(GLfloat angle);
			void set_rotate_y(GLfloat angle);
			void rotate_y(GLfloat angle);
			void set_rotate_z(GLfloat angle);
			void rotate_z(GLfloat angle);
			void multiply(Enigma::Matrix4 matrix);
			void set_translate(GLfloat dx, GLfloat dy, GLfloat dz);
			void translate(GLfloat dx, GLfloat dy, GLfloat dz);
		  void operator =(Enigma::Matrix4& matrix);
		  
			void perspective(GLfloat field_of_view, GLfloat aspect_ratio,
			                 GLfloat near_clip, GLfloat far_clip);
		     
		  void frustrum(GLfloat left_clip, GLfloat right_clip,
				            GLfloat bottom_clip, GLfloat top_clip,
				            GLfloat near_clip, GLfloat far_clip);

			GLfloat* array();    // Get pointer to matrix element array.

		private:
			// Private data.
			
			static constexpr GLfloat PI = 3.1415926535f;  // Pi constant.
			static const int TOTAL = 16;                  // Total array elements.
			GLfloat m_array[Matrix4::TOTAL];              // Matrix element array.
	};
}

#endif // __ENIGMA_MATRIX4_H__
