#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "Animation.h"
#ifndef ANIMATOR_H
#define ANIMATOR_H

class Animator
{
public:
    Animator(Animation* Animation)
    {
        m_CurrentTime = 0.0;
//        m_CurrentAnimation = currentAnimation;
        m_CurrentAnimation = Animation;

        m_FinalBoneMatrices.reserve(10);

        for (int i = 0; i < 10; i++)
            m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
    }

    Animator() {}

    void UpdateAnimation(float dt)
    {
        m_DeltaTime = dt;
        if (m_CurrentAnimation)
        {
            //spdlog::error("idk: {}", m_CurrentAnimation->GetTicksPerSecond());
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

    void BlendTwoAnimations(Animation* pBaseAnimation, Animation* pLayeredAnimation, float blendFactor, float deltaTime)
    {
        // Speed multipliers to correctly transition from one animation to another
        float a = 1.0f;
        float b = pBaseAnimation->GetDuration() / pLayeredAnimation->GetDuration();
        const float animSpeedMultiplierUp = (1.0f - blendFactor) * a + b * blendFactor; // Lerp

        a = pLayeredAnimation->GetDuration() / pBaseAnimation->GetDuration();
        b = 1.0f;
        const float animSpeedMultiplierDown = (1.0f - blendFactor) * a + b * blendFactor; // Lerp

        // Current time of each animation, "scaled" by the above speed multiplier variables
        static float currentTimeBase = 0.0f;
        currentTimeBase += pBaseAnimation->GetTicksPerSecond() * deltaTime * animSpeedMultiplierUp;
        currentTimeBase = fmod(currentTimeBase, pBaseAnimation->GetDuration());

        static float currentTimeLayered = 0.0f;
        currentTimeLayered += pLayeredAnimation->GetTicksPerSecond() * deltaTime * animSpeedMultiplierDown;
        currentTimeLayered = fmod(currentTimeLayered, pLayeredAnimation->GetDuration());

        CalculateBlendedBoneTransform(pBaseAnimation, &pBaseAnimation->GetRootNode(), pLayeredAnimation, &pLayeredAnimation->GetRootNode(), currentTimeBase, currentTimeLayered, glm::mat4(1.0f), blendFactor);
    }


// Recursive function that sets interpolated bone matrices in the 'm_FinalBoneMatrices' vector
    void CalculateBlendedBoneTransform(
            Animation* pAnimationBase,  const AssimpNodeData* node,
            Animation* pAnimationLayer, const AssimpNodeData* nodeLayered,
            const float currentTimeBase, const float currentTimeLayered,
            const glm::mat4& parentTransform,
            const float blendFactor)
    {
        const std::string& nodeName = node->name;

        glm::mat4 nodeTransform = node->transformation;
        Bone* pBone = pAnimationBase->FindBone(nodeName);
        if (pBone)
        {
            pBone->Update(currentTimeBase);
            nodeTransform = pBone->GetLocalTransform();
        }

        glm::mat4 layeredNodeTransform = nodeLayered->transformation;
        pBone = pAnimationLayer->FindBone(nodeName);
        if (pBone)
        {
            pBone->Update(currentTimeLayered);
            layeredNodeTransform = pBone->GetLocalTransform();
        }

        // Blend two matrices
        const glm::quat rot0 = glm::quat_cast(nodeTransform);
        const glm::quat rot1 = glm::quat_cast(layeredNodeTransform);
        const glm::quat finalRot = glm::slerp(rot0, rot1, blendFactor);
        glm::mat4 blendedMat = glm::mat4_cast(finalRot);
        blendedMat[3] = (1.0f - blendFactor) * nodeTransform[3] + layeredNodeTransform[3] * blendFactor;

        glm::mat4 globalTransformation = parentTransform * blendedMat;

        const auto& boneInfoMap = pAnimationBase->GetBoneIDMap();
        if (boneInfoMap.find(nodeName) != boneInfoMap.end())
        {
            const int index = boneInfoMap.at(nodeName).id;
            const glm::mat4& offset = boneInfoMap.at(nodeName).offset;

            m_FinalBoneMatrices[index] = globalTransformation * offset;
        }

        for (size_t i = 0; i < node->children.size(); ++i)
            CalculateBlendedBoneTransform(pAnimationBase, &node->children[i], pAnimationLayer, &nodeLayered->children[i], currentTimeBase, currentTimeLayered, globalTransformation, blendFactor);
    }

private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    Animation* m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;
};
#endif