#version 460 core

struct GLMaterial
{
    sampler2D Diffuse;
    sampler2D Specular;

    float Shininess;
};

struct GLDirectionalLight
{
    vec3 Direction;

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

struct GLPointLight
{
    vec3 Position;

    float MaxRadius;
    float Intensity;

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCoords;

out vec4 FragColor;
  
uniform int _NrOfPointLights;
uniform int _NrOfDirectionalLights;
uniform vec3 _ViewPos;
uniform GLMaterial _Material;

#define MAX_NR_OF_POINT_LIGHTS 10
#define MAX_NR_OF_DIRECTIONAL_LIGHTS 3
uniform GLPointLight _PointLights[MAX_NR_OF_POINT_LIGHTS];
uniform GLDirectionalLight _DirectionalLights[MAX_NR_OF_DIRECTIONAL_LIGHTS];

float GetAttenuationRadius(float Distance, float MaxRadius)
{
    float Falloff = Distance / MaxRadius;
    Falloff *= Falloff;
    Falloff = clamp(1.0 - Falloff * Falloff, 0.0, 1.0);
    Falloff *= Falloff;

    float Attenuation = Falloff;
    Attenuation *= 1.0 / (Distance * Distance + 1.0);

    return Attenuation;
}

vec3 CalculateAmbient(vec3 Ambient)
{
    return texture(_Material.Diffuse, vTexCoords).rgb * Ambient;
}

vec3 CalculateDiffuse(vec3 Normal, vec3 LightDirection, vec3 Diffuse)
{
    float Diff = max(dot(Normal, LightDirection), 0.0);

    return Diff * texture(_Material.Diffuse, vTexCoords).rgb * Diffuse;
}

vec3 CalculateSpecular(vec3 ViewDirection, vec3 Normal, vec3 LightDirection, vec3 Specular)
{
    vec3 ReflectDirection = reflect(-LightDirection, Normal);
    float SpecularVal = pow(max(dot(ViewDirection, ReflectDirection), 0.0), _Material.Shininess);

    return texture(_Material.Specular, vTexCoords).rgb * SpecularVal * Specular;
}

vec3 CalculateDirectionalLights(vec3 Normal, vec3 ViewDirection)
{
    vec3 Result;

    for (int i = 0; i < _NrOfDirectionalLights; ++i)
    {
        vec3 LightDirection = -_DirectionalLights[i].Direction;

        Result += CalculateAmbient(_DirectionalLights[i].Ambient) + 
                    CalculateDiffuse(Normal, LightDirection, _DirectionalLights[i].Diffuse) + 
                    CalculateSpecular(ViewDirection, Normal, LightDirection, _DirectionalLights[i].Specular); 
    }

    return Result;
}

vec3 CalculatePointLights(vec3 Normal, vec3 ViewDirection)
{
    vec3 Result;

    for (int i = 0; i < _NrOfPointLights; ++i)
    {
        float Distance = length(_PointLights[i].Position - vPos);
        float Attenuation = GetAttenuationRadius(Distance, _PointLights[i].MaxRadius);
        vec3 LightDirection = normalize(_PointLights[i].Position - vPos);

        Result += (CalculateAmbient(_PointLights[i].Ambient) * Attenuation) +
                    (
                        (
                            (CalculateDiffuse(Normal, LightDirection, _PointLights[i].Diffuse) * Attenuation) + 
                            (CalculateSpecular(ViewDirection, Normal, LightDirection, _PointLights[i].Specular) * Attenuation)
                        )
                        * _PointLights[i].Intensity
                    ) ;
    }

    return Result;
}

void main()
{
    vec3 Normal = normalize(vNormal);
    vec3 ViewDirection = normalize(_ViewPos - vPos);
    
    vec3 Result = CalculateDirectionalLights(Normal, ViewDirection);
    Result += CalculatePointLights(Normal, ViewDirection);

    FragColor = vec4(Result, 1.0);
}