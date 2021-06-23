// OpenGL vertex shader code.

#version 330
in vec4 vPosition;
in vec4 vColour;
uniform mat4 mProjection;
uniform mat4 mTransform;
out vec4 fcolour;

void main()
{
  gl_Position = mProjection * mTransform * vPosition;
  fcolour = vColour;
}