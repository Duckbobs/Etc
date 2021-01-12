#pragma once

#include "Loader.h"
#include <glm/glm.hpp> // ���� ��� ����
#include <vector>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    unsigned int vertexID;
    float weight;
};
struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    /*  Mesh ������  */
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    /*  �Լ�         */
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(ShaderProgram *shaderProgram);
    void Draw();
private:

private:
    /*  ���� ������  */
    unsigned int VAO, VBO, EBO;
    /*  �Լ�         */
    void setupMesh();
};

class Model
{
public:
    /*  �Լ�   */
    Model(char* path)
    {
        loadModel(path);
    }
    void Draw(ShaderProgram *shader);
    void Draw();

private:
    /*  Model ������  */
    std::vector<Mesh> meshes;
    std::string directory;
    /*  �Լ�   */
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
};



/*
for (unsigned int i = 0; i < mesh->mNumBones; i++) {
    std::cout << mesh->mName.C_Str() << std::endl;
}

int numBones = 0;
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
            vertices.push_back(vertex);
        }
    }
}*/