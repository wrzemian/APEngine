//
// Created by micha on 29.03.2023.
//

#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <cmath>
#include "Model.h"

#include <iostream>

class Transform
{
public:
    glm::vec3 _position{};
    glm::vec3 _scale{};
    glm::vec3 _rotation{};
    glm::mat4 _model{};
    glm::mat4 _localTransform{};

    Transform* parent{};
    std::vector<Transform*> children;

    glm::mat4 m_modelMatrix = glm::mat4(1.0f);

    bool m_isDirty = true;

    Transform() {
        _position = glm::vec3(0,0,0);
        _scale = glm::vec3(1,1,1);
        _rotation = glm::vec3(0,0,0);
        //std::cout<<"transform constructor \n";
    }

    Transform( glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
        _position = position;
        _rotation = rotation;
        _scale = scale;
    }

    void setModel(glm::mat4 model) {
        _model = model;
    }

    void setPosition(glm::vec3 newPosition) {
        _position = newPosition;
        _position.x += parent->_position.x;
        _position.y += parent->_position.y;
        _position.z += parent->_position.z;
    }

    void setRotation(glm::vec3 newRotation) {
        _rotation = newRotation;
    }

    glm::mat4 getModel() {
        return _model;
    }

    void addChild(Transform *child) {
        children.push_back(child);
        child->parent = this;
    }

    void updateLocalTransform() {
        glm::mat4 transform = glm::mat4(1.0f);

        transform = glm::translate(transform, _position);
        transform = glm::rotate(transform, _rotation.y, glm::vec3(0.0, 1.0, 0.0));
        transform = glm::rotate(transform, _rotation.x, glm::vec3(1.0, 0.0, 0.0));
        transform = glm::rotate(transform, _rotation.z, glm::vec3(0.0, 0.0, 1.0));
        transform = glm::scale(transform, _scale);

//        std::cout<<_position.x <<"\n"
//                    <<_rotation.x<<"\n"
//                    <<_scale.x<<"\n\n";

        _localTransform = transform;
    }

    void updateWorldTransform(glm::mat4 modelLok, Shader &shader) {

        this->updateLocalTransform();

        this->_model = modelLok * _localTransform;

        for (Transform* child : children) {
            child->updateWorldTransform(this->_model, shader);
        }
        shader.use();
        shader.setMat4("model", this->_model);
    }

    glm::mat4 getLocalModelMatrix()
    {
        const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        // Y * X * Z
        const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

        // translation * rotation * scale (also know as TRS matrix)
        return glm::translate(glm::mat4(1.0f), _position) * rotationMatrix * glm::scale(glm::mat4(1.0f), _scale);
    }

    void computeModelMatrix()
    {
        m_modelMatrix = getLocalModelMatrix();
        m_isDirty = false;
    }

    void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
    {
        m_modelMatrix = parentGlobalModelMatrix * getLocalModelMatrix();
        m_isDirty = false;
    }

    const glm::mat4& getModelMatrix() const
    {
        return m_modelMatrix;
    }

    glm::vec3 getRight() const
    {
        return m_modelMatrix[0];
    }


    glm::vec3 getUp() const
    {
        return m_modelMatrix[1];
    }

    glm::vec3 getBackward() const
    {
        return m_modelMatrix[2];
    }

    glm::vec3 getForward() const
    {
        return -m_modelMatrix[2];
    }

    void setLocalPosition(const glm::vec3& newPosition)
    {
        _position = newPosition;
        m_isDirty = true;
    }

    void setLocalRotation(const glm::vec3& newRotation)
    {
        _rotation = newRotation;
        m_isDirty = true;
    }

    void setLocalScale(const glm::vec3& newScale)
    {
        _scale = newScale;
        m_isDirty = true;
    }
};

