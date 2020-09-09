#version 330 core

const float PI = 3.14159265359;

const int num_lights = 1;

struct Light {
    vec3 direction;
    vec3 radiance;
};

in vec3 frag_position;
in vec2 frag_texcoord;
in vec3 frag_normal;

uniform sampler2D albedo_texture;
uniform sampler2D normal_texture;
uniform sampler2D metalness_texture;
uniform sampler2D roughness_texture;
uniform samplerCube irradiance_map;
uniform samplerCube prefilter_map;
uniform sampler2D brdf_lut_map;

uniform Light light;
uniform vec3 camera_position;

out vec4 colour;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;

    float NdotH = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0f);
    float k = (r * r) / 8.0f;

    float num = NdotV;
    float denom = NdotV * (1.0f - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(1.0f - cosTheta, 5.0f);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
    vec3 albedo = texture(albedo_texture, frag_texcoord).rgb;
    vec3 normal = texture(normal_texture, frag_texcoord).rgb;
    float metalness = texture(metalness_texture, frag_texcoord).r;
    float roughness = texture(roughness_texture, frag_texcoord).r;

    vec3 N = normalize(frag_normal);
    vec3 V = normalize(camera_position - frag_position);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04f); // Fdielectric
    F0 = mix(F0, albedo, metalness);

    // Direct Lightning  // Just one direct light for now (point lights later)
    vec3 direct_lighting = vec3(0.0);
    {
        vec3 L = normalize(-light.direction);
        vec3 H = normalize(V + L);

        // BRDF
        vec3 F = FresnelSchlickRoughness(max(dot(H, V), 0.0f), F0, roughness);
        float D = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);

        // Cook-Torrance
        vec3 num = F * D * G;
        float denom = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f);
        vec3 specular = num / max(denom, 0.00001f);

        vec3 kd = (1.0 - F) * (1.0 - metalness);
        vec3 diffuse = kd * albedo;

        float NdotL = max(dot(N, L), 0.0f);

        //direct_lighting += (diffuse / PI + specular) * light.radiance * NdotL ;
        direct_lighting += (diffuse + specular) * light.radiance * NdotL;
    }

    // IBL
    vec3 ambient_lighting = vec3(0.0);
    {
        vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
        vec3 kd = (1.0 - F) * (1.0 - metalness);

        vec3 irradiance = texture(irradiance_map, N).rgb;
        vec3 diffuse = irradiance * albedo;

        const float MAX_REFLECTION_LOD = 4.0;
        vec3 prefiltered_colour = textureLod(prefilter_map, R,  roughness * MAX_REFLECTION_LOD).rgb;
        vec2 env_brdf  = texture(brdf_lut_map, vec2(max(dot(N, V), 0.0), roughness)).rg;
        vec3 specular = prefiltered_colour * (F * env_brdf.x + env_brdf.y);

        ambient_lighting = (kd * diffuse + specular);// * ao;
    }

    colour = vec4(direct_lighting + ambient_lighting, 1.0);
}

