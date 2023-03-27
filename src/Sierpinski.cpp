#ifndef SIERPINSKI_H
#define SIERPINSKI_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <Shader.cpp>

class Sierpinski {

private:
    unsigned int currentDepth = 0;
    std::vector < std::vector<glm::mat4>> allTranslations;

public:
    const unsigned int MAX_DEPTH = 8;

    Sierpinski() {}
    ~Sierpinski() {}

    void calculateAllTransformations() {
        for (int i = 0; i <= MAX_DEPTH; i++) {
            std::vector<glm::mat4> temp;
            allTranslations.push_back(temp);
        }

        for (int i = 0; i <= MAX_DEPTH; i++) {
            currentDepth = i;
            recursion(glm::vec3(0.0f, -5.0f, 0.0f), 8.0f, i);
            std::cout << "DEPTH "
                      << i
                      << ": calculated transformations for "
                      << allTranslations.at(currentDepth).size()
                      << " pyramids \n";
        }
    }

    void recursion(glm::vec3 position, float scale, int depth) {
        if (depth == 0) {
            glm::mat4 trans = calculatePyramidTransformation(position, scale);
            allTranslations.at(currentDepth).push_back(trans);
        }
        else {
            depth--;
            scale /= 2.0f;

            // left
            recursion(glm::vec3(position.x - scale,
                                position.y,
                                position.z - scale * sqrt(3.0f) / 3.0f),
                      scale, depth);

            //right
            recursion(glm::vec3(position.x + scale,
                                position.y,
                                position.z - scale * sqrt(3.0f) / 3.0f),
                      scale, depth);

            // front
            recursion(glm::vec3(position.x,
                                position.y,
                                position.z + (2.0f * scale * sqrt(3.0f) / 3.0f)),
                      scale, depth);

            // up
            recursion(glm::vec3(position.x,
                                position.y + (2 * scale * sqrt(6.0f) / 3.0f),
                                position.z),
                      scale, depth);
        }
    }

    glm::mat4 calculatePyramidTransformation(glm::vec3 position, float scale) {
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(scale, scale, scale));

        return model;
    }

    void draw(Shader* shader, unsigned int depth) {
        for (glm::mat4 translation : allTranslations.at(depth)) {
            shader->setMat4("translation", &translation);
            glDrawArrays(GL_TRIANGLES, 0, 12);
        }
    }

    const unsigned int getMAX_DEPTH() {
        return MAX_DEPTH;
    }

};

#endif // !
