#version 330 core  
layout (location = 0) in vec3 position;  

uniform mat4 projectionView;
uniform mat4 model;
uniform vec3 offset;

void main()  
{  
    gl_Position = projectionView * model * vec4(position + offset, 1.0f);
}