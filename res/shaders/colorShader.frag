#version 330 core
out vec4 FragColor;

uniform vec3 inputColor;

void main()
{
    FragColor = vec4(inputColor, 1.0);
}