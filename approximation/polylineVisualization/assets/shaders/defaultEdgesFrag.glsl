#version 400

layout(location = 0, index = 0) out vec4 fragColor;
uniform vec4 baseColor;

void main ()
{
   fragColor = baseColor;
}
