#version 330

layout(location = 0) in vec3 vertex_position;

uniform mat4 projection;
uniform mat4 view;

out vec3 frag_position;

void main()
{
    frag_position = vertex_position;
    gl_Position = projection * view * vec4(vertex_position, 1.0);
}
