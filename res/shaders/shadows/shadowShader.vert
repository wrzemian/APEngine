#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in ivec4 boneIds;
layout (location = 6) in vec4 weights;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    mat3 TBN;
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
        for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
        {
            if(boneIds[i] == -1)
            continue;
            if(boneIds[i] >=MAX_BONES)
            {
                totalPosition = vec4(aPos,1.0f);
                break;
            }
            vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos,1.0f);
            totalPosition += localPosition * weights[i];
//            vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * aNormal;
        }


        vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
        vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
        vs_out.TexCoords = aTexCoords;
        vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
        gl_Position = projection * view * model * totalPosition;
    } else {
        vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
        vs_out.Normal = vec3(0,0,0);
        vs_out.TexCoords = aTexCoords;
        vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
        vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
        vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
        vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
        vs_out.TBN = mat3(T, B, N);
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }

}