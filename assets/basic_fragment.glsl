#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

out vec4 colour;

void main()
{
    colour = texture(u_Texture, v_TexCoord);
}
