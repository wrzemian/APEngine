#version 330 core  
in vec3 ourColor;  
in vec2 TexCoord;

out vec4 color;

uniform vec3 uColor;
uniform sampler2D ourTexture;

void main()  
{  
    color = texture(ourTexture, TexCoord) * vec4(uColor, 1.0f);  
}