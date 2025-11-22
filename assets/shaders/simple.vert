#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;

uniform mat3 model;
uniform mat3 view;
uniform mat3 projection;

out vec2 texCoord;

void main()
{
    vec3 worldPos = model * vec3(aPos, 1.0);
    vec3 cameraPos = view * worldPos;
    vec3 clipPos = projection * cameraPos;

    gl_Position = vec4(clipPos.xy, 0.0, 1.0);
    texCoord = aTex;
}