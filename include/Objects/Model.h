#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>

#include "Mesh.h"
#include "../Shader.h"

struct BoneInfo
{
    int id;
    glm::mat4 offset;
};

class Model
{
public:
    Model();
    Model(std::string const &path, bool gamma = false);
    void Draw(Shader &shader);

    std::map<std::string, BoneInfo>& GetBoneInfoMap();
    int& GetBoneCount();

    std::vector<Mesh> meshes;

private:
    std::vector<Texture> textures_loaded;
    std::string directory;
    bool gammaCorrection;

    int Weights;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
    int m_BoneCounter;

    void SetVertexBoneDataToDefault(Vertex& vertex);
    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
};

#endif // MODEL_H
