#include "Triangle.h"

void Triangle::bindBuffers()
{
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Atrybut pozycji  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Atrybut koloru  
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);
}

void Triangle::init()
{
	Shader temp(vertexShaderPath, fragmentShaderPath);
	shader = &temp; // pewnie tu się zesra access pamieci

    bindBuffers();
    shader->use();
}

void Triangle::draw()
{
    
    glBindVertexArray(_VBO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
