////
//// Created by tomek on 28.05.2023.
////
//
//#ifndef APENGINE_BONE_H
//#define APENGINE_BONE_H
//
//#include <string>
//#include "glm/vec3.hpp"
//#include "glm/fwd.hpp"
//#include "glm/detail/type_quat.hpp"
//#include "assimp/anim.h"
//#include "HudAnimation.h"
//#include "GLConverter.cpp"
//
//struct AssimpNodeData{
//    glm::mat4 transform;
//    std::string name;
//    int childrenCount;
//    std::vector<AssimpNodeData> children;
//};
//
//struct KeyPosition
//{
//    glm::vec3 position;
//    float timeStamp;
//};
//
//struct KeyRotation
//{
//    glm::quat orientation;
//    float timeStamp;
//};
//
//class Bone
//{
//public:
//    Bone(std::string name, int ID, const aiNodeAnim* channel);
//    ~Bone();
//
//    void Update(float animationTime, float previousAnimationTime, float blendingTime, const HudAnimation& previousAnimation);
//    void Update(float animationTime);
//
//    const glm::mat4& GetLocalTransform() const;
//    const std::string& GetBoneName() const;
//    int GetBoneID() const;
//
//    void SetId(int id);
//
//private:
//    std::string name;
//    int ID;
//    glm::mat4 localTransform;
//    KeyPosition* positions;
//    unsigned int numPositions;
//    KeyRotation* rotations;
//    unsigned int numRotations;
//
//    int GetPositionIndex(float animationTime) const;
//    int GetRotationIndex(float animationTime) const;
//    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
//    glm::vec3 InterpolatePosition(float animationTime, float previousAnimationTime, float blendingTime, HudAnimation& previousAnimation);
//    glm::mat4 InterpolateRotation(float animationTime, float previousAnimationTime, float blendingTime, HudAnimation& previousAnimation);
//
////    glm::quat GetGLMQuat(aiQuaternion quaterniont);
//    glm::quat GetGLMQuat(aiQuaternion quaterniont);
//};
//
//
//#endif //APENGINE_BONE_H
