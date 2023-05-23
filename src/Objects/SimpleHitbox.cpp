#include "../../include/Objects/SimpleHitbox.h"

SimpleHitbox::SimpleHitbox(std::string fileName) : Hitbox(fileName){
    IGui::setWindowName("SIMPLE");
}

SimpleHitbox::~SimpleHitbox() {

}

void SimpleHitbox::resolveCollision(Hitbox &other) {
    glm::vec3 overlap = currentMin() - other.currentMax();
    glm::vec3 overlapAbs(std::abs(overlap.x), std::abs(overlap.y), std::abs(overlap.z));

    // Find the smallest overlap
    float minOverlap = std::min({overlapAbs.x, overlapAbs.y, overlapAbs.z});

    // Move the object out of the other one along the smallest overlap axis
    if (minOverlap == overlapAbs.x) {
        _object->onCollisionX(other._object);
    } else if (minOverlap == overlapAbs.y) {
        _object->onCollisionY(other._object);
    } else if (minOverlap == overlapAbs.z) {
        _object->onCollisionZ(other._object);
    }
}
