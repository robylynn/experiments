#version 400

in vec4 vertex;
uniform mat4 worldViewProjMatrix;

void main()
{
   gl_Position = worldViewProjMatrix * vertex;
}
