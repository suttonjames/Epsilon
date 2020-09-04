#version 330 core

layout(location = 0) in vec3 vertex_position;

uniform mat4 view;
uniform mat4 projection;

out vec3 frag_texcoord;

void main()
{
	vec4 position = projection * view * vec4(vertex_position, 1.0);
	gl_Position = position.xyww;
	frag_texcoord = vertex_position;
}
