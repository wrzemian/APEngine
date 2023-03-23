#version 330 core  
layout (location = 0) in vec3 position; // zmienna position ma lokalizację 0  
layout (location = 1) in vec3 color; // zmienna color ma lokalizację 1

out vec3 ourColor; // przekaż kolor do FS

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()  
{  
    gl_Position = vec4(position, 1.0);  
	//gl_Position = projection * view * model * vec4(position, 1.0f);
    ourColor = color; // ustaw ourColor na kolor wejściowy z atrybutu wierzchołka  
}

