#version 330 core

in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_Normal;

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
    vec3 ambient = light.ambient * texture(material.diffuse, v_TexCoord).rgb;

    // diffuse
    vec3 norm = normalize(v_Normal);
    vec3 light_direction = normalize(light.position - v_Position);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, v_TexCoord).rgb;

    // specular
    vec3 view_direction = normalize(camera_position - v_Position);
    vec3 reflect_direction = reflect(-light_direction, norm);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, v_TexCoord).rgb;

    // emission
    vec3 emission = vec3(0.0);
    if (texture(material.specular, v_TexCoord).r == 0.0) {
        emission = texture(material.emission, v_TexCoord).rgb;
    }

    vec3 result = ambient + diffuse + specular + emission;

    colour = vec4(result, 1.0);
}
