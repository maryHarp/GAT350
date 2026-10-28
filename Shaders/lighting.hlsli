#pragma once

#define LIGHT_DIRECTIONAL   0
#define LIGHT_POINT         1
#define LIGHT_SPOT          2


struct Material
{
    float4 color;
    float2 tiling;
    float2 offset;
    float shininess;
    
};

struct Light
{
    float3 position;
    uint type;
    float3 direction;
    float innerCutoff;
    float3 color;
    float intensity;
    float3 ambient;
    float outerCutoff;
};

float3 getLightVector(Light light, float3 position, out float attenuation )
{
    float3 L;
    attenuation = 1.0f;
    switch (light.type)
    {
        case LIGHT_DIRECTIONAL:
            L = normalize(light.position);
            break;
        case LIGHT_POINT:
            L = normalize(light.position - position);
            break;
        case LIGHT_SPOT:
            L = normalize(light.position - position);
            attenuation = smoothstep(light.outerCutoff, light.innerCutoff, dot(-L, light.direction));
            break;
            
    }
    return L;
}

float3 calculateDiffuse(float3 radiance, float3 N, float3 L)
{
    return radiance * saturate(dot(N, L));
}

float3 calculateSpecular(float3 radiance, float3 N, float3 L, float3 V, Material material)
{
   float3 specular = float3(0, 0, 0);
    
    if (dot(N, L) > 0)
    {
        // blinn-phong specular
        float3 H = normalize(V + L);
        float HdotN = saturate(dot(H, N));
        HdotN = pow(HdotN, material.shininess);
        specular = radiance * HdotN;
        
    }
    
    return specular;
}