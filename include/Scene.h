//
// Created by micha on 29.03.2023.
//

#pragma once
#include "Transform.h"
#include <vector>

class Scene
{
private:
    std::vector<Transform*> sceneObjects;
    unsigned int ID;
    float rotateSpeed;

public:
    Scene() {}
    ~Scene() {}

    void addObjects(Transform* transform)
    {
        sceneObjects.push_back(transform);
    }

    void UpdateTransform(Shader shader)
    {
        for (int i = 0; i < sceneObjects.size(); i++)
        {
            sceneObjects.at(i)->updateWorldTransform(glm::mat4(1.0f), shader);
        }
    }
};