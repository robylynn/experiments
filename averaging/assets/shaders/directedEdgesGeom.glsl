#version 400

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;
out vec2 tex_coord;

// Position independent line thickness. Assumes gl_Position's are in clip-space
void main()
{
  vec3 vector = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);

  float w0 = gl_in[0].gl_Position.w;
  float w1 = gl_in[1].gl_Position.w;

  if (length(vector) != 0) {
    vec4 binormal = vec4(-vector.y, vector.x, 0, 0);
    normalize(binormal);
    vec4 offset0 = 0.005 * binormal * w0;
    vec4 offset1 = 0.005 * binormal * w1;
    gl_Position = gl_in[0].gl_Position - offset0;
    tex_coord = vec2(0,0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + offset0;
    tex_coord = vec2(1,0);
    EmitVertex();
    gl_Position = gl_in[1].gl_Position - offset1;
    tex_coord = vec2(0,1);
    EmitVertex();
    gl_Position = gl_in[1].gl_Position + offset1;
    tex_coord = vec2(1,1);
    EmitVertex();
    EndPrimitive();
  }
}
