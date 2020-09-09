#version 330 core

in vec3 frag_position;
in vec2 frag_texcoord;
in vec3 frag_normal;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 camera_position;

out vec4 colour;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, frag_texcoord).rgb;

    // diffuse
    vec3 norm = normalize(frag_normal);
    vec3 light_direction = normalize(light.position - frag_position);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, frag_texcoord).rgb;

    // specular
    vec3 view_direction = normalize(camera_position - frag_position);
    vec3 reflect_direction = reflect(-light_direction, norm);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, frag_texcoord).rgb;

    // emission
    vec3 emission = vec3(0.0);
    if (texture(material.specular, frag_texcoord).r == 0.0) {
        emission = texture(material.emission, frag_texcoord).rgb;
    }

    vec3 result = ambient + diffuse + specular + emission;

    colour = vec4(result, 1.0);
}
