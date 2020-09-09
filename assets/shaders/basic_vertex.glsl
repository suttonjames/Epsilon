#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 frag_texcoord;

void main()
{
    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
    frag_texcoord = vertex_texcoord;
}
