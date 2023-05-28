//
// Created by tomek on 26.05.2023.
//

#include "../Objects/Model.h"
#include "glm/fwd.hpp"

#ifndef APENGINE_ANIMATIONMODEL_H
#define APENGINE_ANIMATIONMODEL_H


struct BoneInfo {
    int id;
    glm::mat4 offset;
};

class AnimationModel : public Model {
public:
    AnimationModel(const std::string &path, bool gamma);
    AnimationModel(const Mesh &mesh, std::shared_ptr<Shader> &shader);

    void LoadModel(std::string const &path);
    void SetVertexBoneDataToDefault(Vertex& vertex);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

    std::unordered_map<std::string, BoneInfo> &GetBoneInfoMap();
    uint16_t GetBoneCount();

private:
    std::unordered_map<std::string, BoneInfo> boneInfoMap;
    uint16_t boneCounter = 0;

    glm::vec3 GetGLMVec(aiVector3D t);

    glm::mat4 ConvertMatrixToGLMFormat(aiMatrix4x4 t);

    std::string name;
};

#endif //APENGINE_ANIMATIONMODEL_H
