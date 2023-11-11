#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 vPos;
out vec3 vNormal;
out vec2 vTexCoords;

uniform mat4 _Transform;
uniform mat4 _View;
uniform mat4 _Projection;

void main()
{
    gl_Position = _Projection * _View * _Transform * vec4(aPos, 1.0);

    vPos = vec3(_Transform * vec4(aPos, 1.0));
    vNormal = aNormal;
    vTexCoords = aTexCoords;
} 