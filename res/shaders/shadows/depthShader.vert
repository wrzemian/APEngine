#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 5) in ivec4 boneIds;
layout(location = 6) in vec4 weights;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform bool animated;
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];


void main()
{
    if(animated) {
        vec4 totalPosition = vec4(0.0f);
            totalPosition += finalBonesMatrices[boneIds[0]] * vec4(aPos,1.0f) * weights[0];
            totalPosition += finalBonesMatrices[boneIds[1]] * vec4(aPos,1.0f) * weights[1];
            totalPosition += finalBonesMatrices[boneIds[2]] * vec4(aPos,1.0f) * weights[2];
            totalPosition += finalBonesMatrices[boneIds[3]] * vec4(aPos,1.0f) * weights[3];
        gl_Position = lightSpaceMatrix * model * totalPosition;
    } else {
        gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
    }
}