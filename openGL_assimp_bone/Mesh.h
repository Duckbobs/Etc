#pragma once

#include <glm/glm.hpp> // ���� ��� ����
#include <vector>
#include "Loader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    //glm::vec2 TexCoords;
    float boneWeights[2][4] = { 0.0f };
    float boneIds[2][4] = { 0.0f };
};
/*struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};*/
struct Bone
{
    glm::mat4 boneOffset;
    glm::mat4 finalTransform;
};

class Mesh {
public:
    /*  Mesh ������  */
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    //std::vector<Texture> textures;
    /*  Bone ������  */
    std::vector<Bone> bones;

    /*  �Լ�         */
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices/*, std::vector<Texture> textures*/);
    void Draw(ShaderProgram* shaderProgram);
    //void Draw();
private:

private:
    /*  ���� ������  */
    unsigned int VAO, VBO, EBO;
    /*  �Լ�         */
    void setupMesh();
};