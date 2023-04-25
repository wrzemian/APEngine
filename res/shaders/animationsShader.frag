#version 330 core

in vec2 TexCoord;

uniform float time;
uniform vec4 color;

out vec4 FragColor;

void main()
{
    float pulse = abs(sin(time * 2.0f));
    FragColor = mix(vec4(color.xyz, 0.20), vec4(1.0), pulse);
}