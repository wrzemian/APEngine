#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "Animation.h"

class Animator
{
public:
    Animator() {

    }

    Animator(Animation* Animation)
    {
        m_CurrentTime = 0.0;
//        m_CurrentAnimation = currentAnimation;
        m_CurrentAnimation = Animation;

        m_FinalBoneMatrices.reserve(100);

        for (int i = 0; i < 100; i++)
            m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
    }

    void UpdateAnimation(float dt)
    {
        m_DeltaTime = dt;
        if (m_CurrentAnimation)
        {
            m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
            m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
            CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
        }
    }

    void PlayAnimation(Animation* pAnimation)
    {
        m_CurrentAnimation = pAnimation;
        m_CurrentTime = 0.0f;
    }

    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
    {
        std::string nodeName = node->name;
        glm::mat4 nodeTransform = node->transformation;

        Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

        if (Bone)
        {
            Bone->Update(m_CurrentTime);
            nodeTransform = Bone->GetLocalTransform();
        }

        glm::mat4 globalTransformation = parentTransform * nodeTransform;

        auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
        if (boneInfoMap.find(nodeName) != boneInfoMap.end())
        {
            int index = boneInfoMap[nodeName].id;
            glm::mat4 offset = boneInfoMap[nodeName].offset;
            m_FinalBoneMatrices[index] = globalTransformation * offset;
        }

        for (int i = 0; i < node->childrenCount; i++)
            CalculateBoneTransform(&node->children[i], globalTransformation);
    }

    std::vector<glm::mat4> GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }

    void renderAnimation(const std::string& vertexShader,const std::string& fragmentShader){
//        ourShader.use();
//
//        // view/projection transformations
//        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
//                                                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//        glm::mat4 view = camera.GetViewMatrix();
//        ourShader.setMat4("projection", projection);
//        ourShader.setMat4("view", view);
//
//        auto transforms = animator.GetFinalBoneMatrices();
//        for (int i = 0; i < transforms.size(); ++i)
//            ourShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
//
//        // render the loaded model
//        glm::mat4 model = glm::mat4(1.0f);
//        // translate it down so it's at the center of the scene
//        model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
//        // it's a bit too big for our scene, so scale it down
//        model = glm::scale(model, glm::vec3(.5f, .5f, .5f));
//        ourShader.setMat4("model", model);
//        ourModel.Draw(ourShader);
    };

private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    Animation* m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;
    Shader ourShader;
};