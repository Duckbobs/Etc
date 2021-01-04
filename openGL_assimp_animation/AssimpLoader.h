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
    void Draw(ShaderProgram shaderProgram);
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
    void Draw(ShaderProgram shader);
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