#ifndef APENGINE_FRUSTUMOPITMIZER_H
#define APENGINE_FRUSTUMOPITMIZER_H

#endif //APENGINE_FRUSTUMOPITMIZER_H

#include "Camera.h"

struct Plane
{
    glm::vec3 normal = { 0.f, 1.f, 0.f }; // unit vector
    float     distance = 0.f;        // Distance with origin

    Plane() = default;

    Plane(const glm::vec3& p1, const glm::vec3& norm);

    float getSignedDistanceToPlane(const glm::vec3& point) const;
};



struct Frustum
{
    Plane topFace;
    Plane bottomFace;

    Plane rightFace;
    Plane leftFace;

    Plane farFace;
    Plane nearFace;
};

struct BoundingVolume
{
    virtual bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const = 0;

    virtual bool isOnOrForwardPlane(const Plane& plane) const = 0;

    bool isOnFrustum(const Frustum& camFrustum) const;
};

struct AABB : public BoundingVolume
{
    glm::vec3 center{ 0.f, 0.f, 0.f };
    glm::vec3 extents{ 0.f, 0.f, 0.f };

    AABB(const glm::vec3& min, const glm::vec3& max);

    AABB(const glm::vec3& inCenter, float iI, float iJ, float iK);

    bool isOnOrForwardPlane(const Plane& plane) const final;

    bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const final;
};

namespace Fru {
    Frustum createFrustumFromCamera(const Camera& cam, float aspect, float fovY, float zNear, float zFar);

    AABB generateAABB(const Model& model);
}


class Entity
{
public:
    //Scene graph
    std::list<std::unique_ptr<Entity>> children;
    Entity* parent = nullptr;

    //Space information
    Transform transform;

    Model* pModel = nullptr;
    std::unique_ptr<AABB> boundingVolume;


    // constructor, expects a filepath to a 3D model.
    Entity(Model& model) : pModel{ &model }
    {
        boundingVolume = std::make_unique<AABB>(Fru::generateAABB(model));
        //boundingVolume = std::make_unique<Sphere>(generateSphereBV(model));
    }

    AABB getGlobalAABB()
    {
        //Get global scale thanks to our transform
        const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(boundingVolume->center, 1.f) };

        // Scaled orientation
        const glm::vec3 right = transform.getRight() * boundingVolume->extents.x;
        const glm::vec3 up = transform.getUp() * boundingVolume->extents.y;
        const glm::vec3 forward = transform.getForward() * boundingVolume->extents.z;

        const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
                            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
                            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

        const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
                            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
                            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

        const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
                            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
                            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

        return AABB(globalCenter, newIi, newIj, newIk);
    }

    //Add child. Argument input is argument of any constructor that you create. By default you can use the default constructor and don't put argument input.
    template<typename... TArgs>
    void addChild(TArgs&... args)
    {
        children.emplace_back(std::make_unique<Entity>(args...));
        children.back()->parent = this;
    }

    //Update transform if it was changed
    void updateSelfAndChild()
    {
        if (transform.m_isDirty) {
            forceUpdateSelfAndChild();
            return;
        }

        for (auto&& child : children)
        {
            child->updateSelfAndChild();
        }
    }

    //Force update of transform even if local space don't change
    void forceUpdateSelfAndChild()
    {
        if (parent)
            transform.computeModelMatrix(parent->transform.getModelMatrix());
        else
            transform.computeModelMatrix();

        for (auto&& child : children)
        {
            child->forceUpdateSelfAndChild();
        }
    }


    void drawSelfAndChild(const Frustum& frustum, Shader& ourShader, unsigned int& display, unsigned int& total)
    {
        if (boundingVolume->isOnFrustum(frustum, transform))
        {
            ourShader.setMat4("model", transform.getModelMatrix());
            pModel->Draw(ourShader);
            display++;
        }
        total++;

        for (auto&& child : children)
        {
            child->drawSelfAndChild(frustum, ourShader, display, total);
        }
    }
};