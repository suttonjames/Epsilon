#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_texcoord;

out vec2 frag_texcoord;

void main()
{
    frag_texcoord = vertex_texcoord;
    gl_Position = vec4(vertex_position, 1.0);
}
