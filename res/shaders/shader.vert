#version 330 core  
layout (location = 0) in vec3 position;  
layout (location = 1) in vec2 texCoord;
 

out vec3 ourColor;  
out vec2 TexCoord;

uniform  mat4 projectionView;
uniform mat4 rotation;
uniform mat4 translation;

void main()  
{  
    gl_Position = projectionView * rotation * translation * vec4(position, 1.0f);  
    TexCoord = texCoord;  
}