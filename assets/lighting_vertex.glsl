#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_texcoord;
layout(location = 2) in vec3 vertex_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 frag_position;
out vec2 frag_texcoord;
out vec3 frag_normal;

void main()
{
    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
    frag_position = vec3(model * vec4(vertex_position, 1.0));
    frag_texcoord = vertex_texcoord;
    frag_normal = mat3(transpose(inverse(model))) * vertex_normal;
}
