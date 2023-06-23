#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout(location = 5) in ivec4 boneIds;
layout(location = 6) in vec4 weights;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform bool animated;
const int MAX_BONES = 10;
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


        vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
        vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
        vs_out.TexCoords = aTexCoords;
        vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
        gl_Position = projection * view * model * totalPosition;
    } else {
        vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
        vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
        vs_out.TexCoords = aTexCoords;
        vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }

}