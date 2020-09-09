#version 330 core

in vec3 frag_texcoord;

uniform samplerCube texture;

out vec4 colour;

void main()
{
    colour = texture(texture, frag_texcoord);
}
