#version 400

layout(location = 0, index = 0) out vec4 fragColor;
in vec2 tex_coord;
uniform sampler2D lineTexture;

void main ()
{
  fragColor = texture2D(lineTexture, tex_coord);
}
