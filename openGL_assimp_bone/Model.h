#include <glm/glm.hpp> // ���� ��� ����
#include <vector>

#include "AssimpLoader.h"

class Model {
public:
    /*  �Լ�   */
    Model(char* path);
    void Draw(ShaderProgram* shader);
    void Draw();
private:
    /*  Model ������  */
    std::vector<Mesh> meshes;
};