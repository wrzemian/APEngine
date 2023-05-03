//
// Created by wrzos on 03.05.2023.
//

#include "../include/FrustumOpitmizer.h"

Plane::Plane(const glm::vec3 &p1, const glm::vec3 &norm) : normal(glm::normalize(norm)),
                                                           distance(glm::dot(normal, p1))
{}

float Plane::getSignedDistanceToPlane(const glm::vec3& point) const
{
    return glm::dot(normal, point) - distance;
}

bool BoundingVolume::isOnFrustum(const Frustum& camFrustum) const
{
    return (isOnOrForwardPlane(camFrustum.leftFace) &&
            isOnOrForwardPlane(camFrustum.rightFace) &&
            isOnOrForwardPlane(camFrustum.topFace) &&
            isOnOrForwardPlane(camFrustum.bottomFace) &&
            isOnOrForwardPlane(camFrustum.nearFace) &&
            isOnOrForwardPlane(camFrustum.farFace));
};

AABB::AABB(const glm::vec3& min, const glm::vec3& max)
        : BoundingVolume{}, center{ (max + min) * 0.5f }, extents{ max.x - center.x, max.y - center.y, max.z - center.z }
{}

AABB::AABB(const glm::vec3& inCenter, float iI, float iJ, float iK)
        : BoundingVolume{}, center{ inCenter }, extents{ iI, iJ, iK }
{}

bool AABB::isOnOrForwardPlane(const Plane &plane) const {
    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    const float r = extents.x * std::abs(plane.normal.x) + extents.y * std::abs(plane.normal.y) +
                    extents.z * std::abs(plane.normal.z);

    return -r <= plane.getSignedDistanceToPlane(center);
}

bool AABB::isOnFrustum(const Frustum &camFrustum, const Transform &transform) const {
    //Get global scale thanks to our transform
    const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(center, 1.f) };

    // Scaled orientation
    const glm::vec3 right = transform.getRight() * extents.x;
    const glm::vec3 up = transform.getUp() * extents.y;
    const glm::vec3 forward = transform.getForward() * extents.z;

    const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
                        std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
                        std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

    const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
                        std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
                        std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

    const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
                        std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
                        std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

    const AABB globalAABB(globalCenter, newIi, newIj, newIk);

    return (globalAABB.isOnOrForwardPlane(camFrustum.leftFace) &&
            globalAABB.isOnOrForwardPlane(camFrustum.rightFace) &&
            globalAABB.isOnOrForwardPlane(camFrustum.topFace) &&
            globalAABB.isOnOrForwardPlane(camFrustum.bottomFace) &&
            globalAABB.isOnOrForwardPlane(camFrustum.nearFace) &&
            globalAABB.isOnOrForwardPlane(camFrustum.farFace));
}

namespace Fru {
    Frustum createFrustumFromCamera(const Camera& cam, float aspect, float fovY, float zNear, float zFar)
    {
        Frustum     frustum;
        const float halfVSide = zFar * tanf(fovY * .5f);
        const float halfHSide = halfVSide * aspect;
        const glm::vec3 frontMultFar = zFar * cam.Front;

        frustum.nearFace = { cam.Position + zNear * cam.Front, cam.Front };
        frustum.farFace = { cam.Position + frontMultFar, -cam.Front };
        frustum.rightFace = { cam.Position, glm::cross(frontMultFar - cam.Right * halfHSide, cam.Up) };
        frustum.leftFace = { cam.Position, glm::cross(cam.Up, frontMultFar + cam.Right * halfHSide) };
        frustum.topFace = { cam.Position, glm::cross(cam.Right, frontMultFar - cam.Up * halfVSide) };
        frustum.bottomFace = { cam.Position, glm::cross(frontMultFar + cam.Up * halfVSide, cam.Right) };
        return frustum;
    }

    AABB generateAABB(const Model& model)
    {
        glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
        glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::min());
        for (auto&& mesh : model.meshes)
        {
            for (auto&& vertex : mesh.vertices)
            {
                minAABB.x = std::min(minAABB.x, vertex.Position.x);
                minAABB.y = std::min(minAABB.y, vertex.Position.y);
                minAABB.z = std::min(minAABB.z, vertex.Position.z);

                maxAABB.x = std::max(maxAABB.x, vertex.Position.x);
                maxAABB.y = std::max(maxAABB.y, vertex.Position.y);
                maxAABB.z = std::max(maxAABB.z, vertex.Position.z);
            }
        }
        return AABB(minAABB, maxAABB);
    }
}


