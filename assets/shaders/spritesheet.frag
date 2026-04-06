#version 330 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D tex0;
uniform vec4 tint;

void main()
{
  fragColor = texture(tex0, texCoord) * tint;
}
