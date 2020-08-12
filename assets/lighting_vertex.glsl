#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 v_Position;
out vec2 v_TexCoord;
out vec3 v_Normal;

void main()
{
    gl_Position = projection * view * model * vec4(a_Position, 1.0);
    v_Position = vec3(model * vec4(a_Position, 1.0));
    v_TexCoord = a_TexCoord;
    v_Normal = mat3(transpose(inverse(model))) * a_Normal;
}
