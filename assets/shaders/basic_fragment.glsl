#version 330 core

in vec2 frag_texcoord;

uniform sampler2D texture;

out vec4 colour;

void main()
{
    colour = texture(texture, frag_texcoord);
}
