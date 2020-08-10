#version 330 core

in vec3 v_TexCoord;

uniform samplerCube u_Texture;

out vec4 colour;

void main()
{
    colour = texture(u_Texture, v_TexCoord);
}
