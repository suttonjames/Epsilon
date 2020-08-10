#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_TexCoord;

void main()
{
	vec4 pos = u_Projection * u_View * vec4(a_Position, 1.0);
	gl_Position = pos.xyww;
	v_TexCoord = a_Position;
}