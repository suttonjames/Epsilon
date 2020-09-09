#version 330

in vec3 frag_position;

uniform samplerCube environment_map;

const float PI = 3.14159265359;

out vec4 colour;

void main()
{
    vec3 normal = normalize(frag_position);
    vec3 irradiance = vec3(0.0f);

    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = cross(up, normal);
    up = cross(normal, right);

    float sample_delta = 0.025f;
    float num_samples = 0.0f;

    for (float phi = 0.0f; phi < 2.0f * PI; phi += sample_delta) {
        for (float theta = 0.0f; theta < 0.5f * PI; theta += sample_delta) {
            vec3 tangent_sample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

            vec3 sample_vec = tangent_sample.x * right + tangent_sample.y * up + tangent_sample.z * normal;

            irradiance += texture(environment_map, sample_vec).rgb * cos(theta) * sin(theta);
            num_samples++;
        }
    }

    irradiance = PI * irradiance * (1.0f / float(num_samples));

    colour = vec4(irradiance, 1.0f);
}
