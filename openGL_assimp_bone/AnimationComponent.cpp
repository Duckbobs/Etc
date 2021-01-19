/*#include "AnimationComponent.h"

// Ư�� �ð�(animationTime)�� Ư�� �ִϸ��̼� �ε���(animationIndex)�� �Ѱ��ָ�
// �� Bone�� ���¸� Matrix �������� �����ϴ� �Լ�
// �� �����Ӹ��� ȣ���
std::vector<glm::mat4>* AnimationComponent::ExtractBoneTransforms(float animationTime, const int animationIndex)
{
    // �ִϸ��̼��� ��� �ݺ��ǵ��� fmod ������ ����
    animationTime = fmod(animationTime, scene->mAnimations[animationIndex]->mDuration);
    // root node�� ���� ����� ���ڷ� �Ѱ��ָ� ��� ȣ���� ���Ͽ� boneInfos�� �����͸� �����ϴ� �Լ�
    ReadNodeHierarchy(animationTime, scene->mRootNode, glm::mat4(1.0f));

    for (int i = 0; i < scene->mMeshes[0]->mNumBones; i++)
    {
        // boneTransforms�� vector<mat4> Ÿ������ ũ��� Bone�� ������ ����
        boneTransforms[i] = boneInfos[i].finalTransform;
    }
    return &boneTransforms;
}

// ��� ȣ���� ���� �ڽ� ���� �������鼭 ���� ��Ī�� boneTransformation�� �����ϴ� �Լ�
void AnimationComponent::ReadNodeHierarchy(float animationTime, const aiNode* node, const glm::mat4& parentTransform)
{

    std::string nodeName(node->mName.data);

    const aiAnimation* animation = scene->mAnimations[0];
    glm::mat4 nodeTransform = ToMat4(node->mTransformation);

    const aiNodeAnim* nodeAnim = FindNodeAnim(animation, nodeName);

    // �ִϸ��̼� ������ �ִ� node���
    if (nodeAnim)
    {
        // �־��� key frame�� ������ animationTime ������ �̿��� interpolation�� �ϰ� ���� ����
        const aiVector3D& scaling = CalcInterpolatedValueFromKey(animationTime, nodeAnim->mNumScalingKeys, nodeAnim->mScalingKeys);
        glm::mat4 scalingM = scale(mat4(1.0f), glm::vec3(scaling.x, scaling.y, scaling.z));

        const aiQuaternion& rotationQ = CalcInterpolatedValueFromKey(animationTime, nodeAnim->mNumRotationKeys, nodeAnim->mRotationKeys);
        glm::mat4 rotationM = glm::mat4(quat(rotationQ.w, rotationQ.x, rotationQ.y, rotationQ.z));

        const aiVector3D& translation = CalcInterpolatedValueFromKey(animationTime, nodeAnim->mNumPositionKeys, nodeAnim->mPositionKeys);
        glm::mat4 translationM = translate(mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z));

        nodeTransform = translationM * rotationM * scalingM;
    }

    // globalTransform�� bone space���� ���ǵǾ��� �������� model space���� ���ǵǵ��� ��
    // parentTransform�� �θ� bone space���� ���ǵǾ��� �������� model space���� ���ǵǵ��� ��
    // nodeTransform�� bone space���� ���ǵǾ��� �������� �θ� bone space���� ���ǵǵ��� ��, 
    // Ȥ�� �θ� bone space�� �������� �� ������ ��ȯ
    glm::mat4 globalTransform = parentTransform * nodeTransform;

    // bone�� �ִ� ��忡 ���ؼ��� bone Transform�� ����
    // boneMap�� map<string, int> Ÿ������ bone�� �̸��� index ����
    if (boneMap.find(nodeName) != boneMap.end())
    {
        unsigned int boneIndex = boneMap[nodeName];
        boneInfos[boneIndex].finalTransform =
            ToMat4(scene->mRootNode->mTransformation) *
            globalTransform *
            // boneOffset�� model space���� ���ǵǾ��� �������� bone space���� ���ǵǵ��� ����� ��
            boneInfos[boneIndex].boneOffset;
    }

    // ��� �ڽ� ��忡 ���� ��� ȣ��
    for (int i = 0; i < node->mNumChildren; i++)
        ReadNodeHierarchy(animationTime, node->mChildren[i], globalTransform);
}

aiNodeAnim* AnimationComponent::FindNodeAnim(const aiAnimation* animation, const std::string nodeName)
{
    for (int i = 0; i < animation->mNumChannels; i++)
        if (animation->mChannels[i]->mNodeName.data == nodeName)
            return animation->mChannels[i];
    return nullptr;
}

aiVector3D AnimationComponent::CalcInterpolatedValueFromKey(float animationTime, const int numKeys, const aiVectorKey* const vectorKey) const
{
    aiVector3D ret;
    if (numKeys == 1)
    {
        ret = vectorKey[0].mValue;
        return ret;
    }

    unsigned int keyIndex = FindKeyIndex(animationTime, numKeys, vectorKey);
    unsigned int nextKeyIndex = keyIndex + 1;

    assert(nextKeyIndex < numKeys);

    float deltaTime = vectorKey[nextKeyIndex].mTime - vectorKey[keyIndex].mTime;
    float factor = (animationTime - (float)vectorKey[keyIndex].mTime) / deltaTime;

    assert(factor >= 0.0f && factor <= 1.0f);

    const aiVector3D& startValue = vectorKey[keyIndex].mValue;
    const aiVector3D& endValue = vectorKey[nextKeyIndex].mValue;

    ret.x = startValue.x + (endValue.x - startValue.x) * factor;
    ret.y = startValue.y + (endValue.y - startValue.y) * factor;
    ret.z = startValue.z + (endValue.z - startValue.z) * factor;

    return ret;
}

aiQuaternion AnimationComponent::CalcInterpolatedValueFromKey(float animationTime, const int numKeys, const aiQuatKey* const quatKey) const
{
    aiQuaternion ret;
    if (numKeys == 1)
    {
        ret = quatKey[0].mValue;
        return ret;
    }

    unsigned int keyIndex = FindKeyIndex(animationTime, numKeys, quatKey);
    unsigned int nextKeyIndex = keyIndex + 1;

    assert(nextKeyIndex < numKeys);

    float deltaTime = quatKey[nextKeyIndex].mTime - quatKey[keyIndex].mTime;
    float factor = (animationTime - (float)quatKey[keyIndex].mTime) / deltaTime;

    assert(factor >= 0.0f && factor <= 1.0f);

    const aiQuaternion& startValue = quatKey[keyIndex].mValue;
    const aiQuaternion& endValue = quatKey[nextKeyIndex].mValue;
    aiQuaternion::Interpolate(ret, startValue, endValue, factor);
    ret = ret.Normalize();

    return ret;
}

unsigned int AnimationComponent::FindKeyIndex(const float animationTime, const int numKeys, const aiVectorKey* const vectorKey) const
{
    assert(numKeys > 0);
    for (int i = 0; i < numKeys - 1; i++)
        if (animationTime < (float)vectorKey[i + 1].mTime)
            return i;
    assert(0);
}

unsigned int AnimationComponent::FindKeyIndex(const float animationTime, const int numKeys, const aiQuatKey* const quatKey) const
{
    assert(numKeys > 0);
    for (int i = 0; i < numKeys - 1; i++)
        if (animationTime < (float)quatKey[i + 1].mTime)
            return i;
    assert(0);
}*/