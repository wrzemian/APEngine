//#ifndef ANIMATION_H
//#define ANIMATION_H
//
//#include <string>
//#include <unordered_map>
//#include <memory>
//#include <vector>
//#include <assimp/scene.h>
//#include "Bone.h"
//#include "AnimationModel.h"
//#include "AssimpNodeData.h"
//
//class HudAnimation {
//public:
//    HudAnimation() = default;
//    HudAnimation(std::string name, float mDuration, int mTicksPerSecond);
//
//    HudAnimationion() = default;
//
//    std::shared_ptr<Bone> FindBone(const std::string& name);
//    int GetTicksPerSecond() const;
//    float GetDuration() const;
//    AssimpNodeData& GetRootNode();
//    const std::unordered_map<std::string, BoneInfo>& GetBoneIDMap();
//
//    void ReadMissingBones(const aiAnimation* animation, const std::shared_ptr<AnimationModel>& model);
//    void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);
//    void Recalculate(const std::shared_ptr<AnimationModel>& model);
//
//private:
//    std::string name;
//    float duration;
//    int ticksPerSecond;
//    AssimpNodeData rootNode;
//    std::unordered_map<std::string, std::shared_ptr<Bone>> bones;
//    std::unordered_map<std::string, BoneInfo> boneInfoMap;
//    static const int BONE_NUMBER = 100;
//
//};
//
//#endif // ANIMATION_H
