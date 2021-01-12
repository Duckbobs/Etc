#include <glm/glm.hpp> // ���� ��� ����
#include <vector>
#include "Loader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
    glm::vec3 Normal;
    glm::vec3 Bone;
};
struct Bone {
    unsigned int ID;
    float Weights[];
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
    std::vector<Bone> bones;

    /*  �Լ�         */
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(ShaderProgram* shaderProgram);
    void Draw();
private:

private:
    /*  ���� ������  */
    unsigned int VAO, VBO, EBO, BONE_VBO;
    /*  �Լ�         */
    void setupMesh();
};