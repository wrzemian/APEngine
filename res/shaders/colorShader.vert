#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;


out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projectionView;

void main()
{
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = texCoord;

    gl_Position = projectionView * model * vec4(position, 1.0f);
}