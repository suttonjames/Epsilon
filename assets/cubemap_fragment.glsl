#version 330

in vec3 frag_position;

uniform sampler2D equirectangular_map;

out vec4 colour;

vec2 sample_map(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= vec2(0.1591, 0.3183);
    uv += 0.5;
    return uv;
}

void main()
{
    vec2 uv = sample_map(normalize(frag_position));

    vec3 color = texture(equirectangular_map, uv).rgb;

    colour = vec4(color, 1.0);
}
