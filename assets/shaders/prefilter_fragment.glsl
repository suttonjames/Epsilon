#version 330
#define     CUBEMAP_RESOLUTION      1024.0

in vec3 frag_position;

uniform samplerCube environment_map;
uniform float roughness;

const float PI = 3.14159265359;

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

float RadicalInverseVdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10;
}

vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i) / float(N), RadicalInverseVdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness * roughness;
    float phi = 2.0 * PI * Xi.x;
    float cos_theta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
    float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    vec3 H = vec3(cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta);

    vec3 up = ((abs(N.z) < 0.999) ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0));
    vec3 tangent = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
    vec3 sample_vec = tangent * H.x + bitangent * H.y + N * H.z;

    return normalize(sample_vec);
}

void main()
{
    vec3 N = normalize(frag_position);
    vec3 R = N;
    vec3 V = R;

    vec3 prefiltered_colour = vec3(0.0);
    float total_weight = 0.0;

    const uint MAX_SAMPLES = 1024u;
    for (uint i = 0u; i < MAX_SAMPLES; i++) {
        vec2 Xi = Hammersley(i, MAX_SAMPLES);
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if(NdotL > 0.0) {
            float D = DistributionGGX(N, H, roughness);
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);
            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001f;

            float resolution = CUBEMAP_RESOLUTION;
            float sa_texel  = 4.0 * PI / (6.0 * resolution * resolution);
            float sa_sample = 1.0 / (float(MAX_SAMPLES) * pdf + 0.0001f);
            float mip_level = ((roughness == 0.0) ? 0.0 : 0.5 * log2(sa_sample / sa_texel));

            prefiltered_colour += textureLod(environment_map, L, mip_level).rgb * NdotL;
            total_weight += NdotL;
        }
    }

    prefiltered_colour = prefiltered_colour / total_weight;

    colour = vec4(prefiltered_colour, 1.0);
}
