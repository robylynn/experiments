#version 120

attribute vec4 position;
uniform mat4 worldViewProj;

void main()
{
   gl_Position = worldViewProj * position;
}
