#pragma once

#include <glm/glm.hpp> // 순서 상관 없음
#include <vector>

#include "AssimpLoader.h"


class Model {
public:
    ModelData * modelData;
    /*  함수   */
    ~Model();
    Model(char* path);
    void Draw(ShaderProgram* shader);
    //void Draw();
    int BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms, std::vector<glm::fdualquat>& dqs);
    void ReadNodeHeirarchy(const aiScene* scene, float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform, const glm::fdualquat& ParentDQ, glm::vec3 startpos);
    float getDuration();
private:
    glm::mat4 CalcBoneTransform(aiAnimation* anim);
    void CalcInterpolatedRotaion(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
};