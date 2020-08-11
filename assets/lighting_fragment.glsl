#version 330 core

in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_Normal;

uniform sampler2D u_Texture;
uniform vec3 u_LightColour;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;

out vec4 colour;

void main()
{
    // colour of texture
    vec4 object_colour = texture(u_Texture, v_TexCoord);
    //vec4 object_colour = vec4(0.2, 0.6, 0.3, 1.0);

    // ambient
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * u_LightColour;

    // diffuse
    vec3 norm = normalize(v_Normal);
    vec3 light_direction = normalize(u_LightPosition - v_Position);

    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diff * u_LightColour;

    // specular
    float specularStrength = 0.5;

    vec3 view_direction = normalize(u_CameraPosition - v_Position);
    vec3 reflect_direction = reflect(-light_direction, norm);

    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular = specularStrength * spec * u_LightColour;

    vec3 result = (ambient + diffuse + specular) * vec3(object_colour);

    colour = vec4(result, 1.0);
}
