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
  
uniform vec3 _ViewPos;
uniform GLMaterial _Material;
uniform GLPointLight _PointLights[];
uniform GLDirectionalLight _DirectionalLights[];

const int PointLightType = 0;
const int DirectionalLightType = 1;

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

vec3 CalculateAmbient(int LightType, float Attenuation)
{
    if (LightType == PointLightType)
    {
        return texture(_Material.Diffuse, vTexCoords).rgb * _PointLights[0].Ambient * Attenuation;
    }
    else
    {
        return texture(_Material.Diffuse, vTexCoords).rgb * _DirectionalLights[0].Ambient;
    }
}

vec3 CalculateDiffuse(int LightType, vec3 Normal, vec3 LightDirection, float Attenuation)
{
    float Diff = max(dot(Normal, LightDirection), 0.0);

    FragColor.rgb = Diff * texture(_Material.Diffuse, vTexCoords).rgb * _PointLights[0].Diffuse * Attenuation;

    if (LightType == PointLightType)
    {
        return Diff * texture(_Material.Diffuse, vTexCoords).rgb * _PointLights[0].Diffuse * Attenuation;
    }
    else
    {
        return Diff * texture(_Material.Diffuse, vTexCoords).rgb * _DirectionalLights[0].Diffuse;
    }
}

vec3 CalculateSpecular(int LightType, vec3 Normal, vec3 LightDirection, float Attenuation)
{
    vec3 ViewDirection = normalize(_ViewPos - vPos);
    vec3 ReflectDirection = reflect(-LightDirection, Normal);
    float Specular = pow(max(dot(ViewDirection, ReflectDirection), 0.0), _Material.Shininess);

    if (LightType == PointLightType)
    {
       return texture(_Material.Specular, vTexCoords).rgb * Specular * _PointLights[0].Specular * Attenuation;
    }
    else
    {
        return texture(_Material.Specular, vTexCoords).rgb * Specular * _DirectionalLights[0].Specular;
    }
}

void main()
{
    int LightType = PointLightType;
    // int LightType = DirectionalLightType;

    float Distance = length(_PointLights[0].Position - vPos);
    float Attenuation = GetAttenuationRadius(Distance, _PointLights[0].MaxRadius);

    vec3 Normal = normalize(vNormal);

    // FragColor = vec4(Attenuation, Attenuation, Attenuation, 1.0);
    // return;

    vec3 LightDirection = normalize(_PointLights[0].Position - vPos);
    // vec3 LightDirection = normalize(-_DirectionalLights[0].Direction);

    vec3 result = CalculateAmbient(LightType, Attenuation) + ((CalculateDiffuse(LightType, Normal, LightDirection, Attenuation) +
    CalculateSpecular(LightType, Normal, LightDirection, Attenuation)) * _PointLights[0].Intensity);

    FragColor = vec4(result, 1.0);
}