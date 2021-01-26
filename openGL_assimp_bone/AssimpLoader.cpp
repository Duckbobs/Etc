#include "AssimpLoader.h"

AssimpLoader::AssimpLoader(ModelData* modelData, std::string path)
{
    AssimpLoader::loadModel(modelData, path);
}
void AssimpLoader::loadModel(ModelData* modelData, std::string path)
{
    this->modelData = modelData;
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    modelData->scene = scene;

    ///////////////////////////////////////////////////////// ���⼱ �����µ�
    unsigned int numPosKeys = modelData->scene->mAnimations[0]->mChannels[0]->mNumPositionKeys;

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    //directory = path.substr(0, path.find_last_of('/'));

    /*if (scene->HasAnimations()) {
        for (unsigned int i = 0; i < scene->mNumAnimations; i++)
        {
            animations->push_back(scene->mAnimations[i]);
        }
    }*/
    loadBones(scene->mRootNode, scene);
    modelData->m_BoneInfo.resize(modelData->Bone_Mapping.size());

    processNode(scene->mRootNode, scene);
}

void AssimpLoader::processNode(aiNode* node, const aiScene* scene)
{
    // ����� ��� mesh���� ó��(���� �ִٸ�)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        modelData->total_vertices += mesh->mNumVertices;
        modelData->meshes.push_back(processMesh(mesh, scene));
    }
    // �׷� ���� �� �ڽĵ鿡�Ե� �����ϰ� ����
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        AssimpLoader::processNode(node->mChildren[i], scene);
    }
}

Mesh AssimpLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    //std::vector<Texture> textures;

    modelData->Bones.resize(modelData->total_vertices);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // vertex ��ġ, ����, �ؽ�ó ��ǥ�� ó��
        glm::vec3 vector;
        // vertex ��ġ
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // ����
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        // �ؽ�ó ��ǥ
        /*
        if (mesh->mTextureCoords[0]) // mesh�� �ؽ�ó ��ǥ�� ������ �ִ°�?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        */
        vertices.push_back(vertex);
    }
    // indices ó��
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // material ó��
    /*if (mesh->mMaterialIndex >= 0)
    {
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
                aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<Texture> specularMaps = loadMaterialTextures(material,
                aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }
    }*/
    /*int numBones = 0;
    for (int i = 0; i < mesh->mNumBones; i++)
    {
        unsigned int boneIndex = numBones++;
        for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
        {
            unsigned int vertexId = mesh->mBones[i]->mWeights[j].mVertexId;
            float weight = mesh->mBones[i]->mWeights[j].mWeight;
            // ������ �ִ� 8���� Bone�� ������ �ް� ��
            // 2���� 4���� ���͸� �̿��Ͽ� ���� ����
            for (int k = 0; k < 8; k++)
            {
                // ������ �ε���
                unsigned int vectorId = k / 4;
                // �� ������ ���� �ε���
                unsigned int elementId = k % 4;
                // push_back ȿ���� ����
                if (vertices[vertexId].boneWeights[vectorId][elementId] == 0.0f)
                {
                    vertices[vertexId].boneIds[vectorId][elementId] = boneIndex;
                    vertices[vertexId].boneWeights[vectorId][elementId] = weight;
                    break;
                }
            }
        }
    }*/
    loadMeshBones(mesh, modelData->Bones, scene);
    return Mesh::Mesh(vertices, indices/*, textures*/);
}

void AssimpLoader::loadBones(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        std::string NodeName(node->mChildren[i]->mName.data);
        if (NodeName.find(":") != std::string::npos) {
            std::string BoneName = NodeName;
            unsigned int BoneIndex = 0;

            if (modelData->Bone_Mapping.find(BoneName) == modelData->Bone_Mapping.end()) {
                BoneIndex = modelData->m_NumBones;
                modelData->m_NumBones++;
                modelData->Bone_Mapping[BoneName] = BoneIndex;
            }
        }
        if (NodeName != "Body" && NodeName != "metarig" && NodeName != "parasiteZombie" && NodeName != "Armature" && NodeName != "MutantMesh" && NodeName != "Cylinder") {
            std::string BoneName = NodeName;
            unsigned int BoneIndex = 0;

            if (modelData->Bone_Mapping.find(BoneName) == modelData->Bone_Mapping.end()) {
                BoneIndex = modelData->m_NumBones;
                modelData->m_NumBones++;
                modelData->Bone_Mapping[BoneName] = BoneIndex;
            }
        }
        //only uncomment if we need to load cylinder model
    /*else {
    string BoneName(node->mChildren[i]->mName.data);
    unsigned int BoneIndex = 0;
    if (NodeName != "parasiteZombie" || NodeName != "Armature") {
        if (Bone_Mapping.find(BoneName) == Bone_Mapping.end()) {
            BoneIndex = m_NumBones;
            m_NumBones++;
            Bone_Mapping[BoneName] = BoneIndex;
        }
    }

}*/
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        loadBones(node->mChildren[i], scene);
}

void AssimpLoader::loadMeshBones(aiMesh* mesh, std::vector<VertexBoneData>& VertexBoneData, const aiScene* scene)
{
    for (unsigned int i = 0; i < mesh->mNumBones; i++)
    {
        unsigned int BoneIndex = 0;
        std::string BoneName(mesh->mBones[i]->mName.data);

        std::cout << BoneName << std::endl;


        if (modelData->Bone_Mapping.find(BoneName) != modelData->Bone_Mapping.end())
        {
            BoneIndex = modelData->Bone_Mapping[BoneName];
            //BoneInfo bi;
            //m_BoneInfo.push_back(bi);

            aiMatrix4x4 tp1 = mesh->mBones[i]->mOffsetMatrix;
            modelData->m_BoneInfo[BoneIndex].offset = glm::transpose(glm::make_mat4(&tp1.a1));
        }
        else {
            std::cout << "error" << std::endl;
        }

        int nn = mesh->mBones[i]->mNumWeights;
        for (unsigned int n = 0; n < nn; n++) {
            unsigned int vid = mesh->mBones[i]->mWeights[n].mVertexId + modelData->NumVertices;//absolute index
            float weight = mesh->mBones[i]->mWeights[n].mWeight;
            VertexBoneData[vid].AddBoneData(BoneIndex, weight);
        }
        loadAnimations(scene, BoneName, modelData->Animations);
    }
    modelData->NumVertices += mesh->mNumVertices;
}
void AssimpLoader::loadAnimations(const aiScene* scene, std::string BoneName, std::map<std::string, std::map<std::string, const aiNodeAnim*>>& animations)
{
    for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
        const aiAnimation* pAnimation = scene->mAnimations[i];
        std::string animName = pAnimation->mName.data;
        for (unsigned int j = 0; j < pAnimation->mNumChannels; j++) {
            std::string name = pAnimation->mChannels[j]->mNodeName.data;
            if (name == BoneName) {

                animations[animName][BoneName] = pAnimation->mChannels[j];
                break;
            }
            /*if (subname == BoneName) {
                std::cout << "Anim= " << subname << std::endl;
                animations[animName][BoneName] = pAnimation->mChannels[j];
                break;
            }*/
        }
    }
}