#version 460 core

struct GLMaterial
{
    sampler2D Diffuse;
    sampler2D Specular;

    float Shininess;
};

struct GLLight
{
    vec3 Position;

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
uniform GLLight _Light;

vec3 CalculateAmbient()
{
    // for now diffuse and ambient are the same
    return vec3(texture(_Material.Diffuse, vTexCoords)) * _Light.Ambient;
}

vec3 CalculateDiffuse(vec3 normal, vec3 lightDirection)
{
    float diff = max(dot(normal, lightDirection), 0.0);

    return diff * vec3(texture(_Material.Diffuse, vTexCoords)) * _Light.Diffuse;
}

vec3 CalculateSpecular(vec3 normal, vec3 lightDirection)
{
    vec3 viewDirection = normalize(_ViewPos - vPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), _Material.Shininess);

    return vec3(texture(_Material.Specular, vTexCoords)) * specular * _Light.Specular;
}

void main()
{
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(_Light.Position - vPos);

    vec3 result = CalculateAmbient() + CalculateDiffuse(normal, lightDirection) + CalculateSpecular(normal, lightDirection);

    FragColor = vec4(result, 1.0);
}