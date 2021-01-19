#include "AssimpLoader.h"

AssimpLoader::AssimpLoader(ModelData* modelData, std::string path)
{
    AssimpLoader::loadModel(&modelData->meshes, &modelData->animations, path);
}
void AssimpLoader::loadModel(std::vector<Mesh>* meshes, std::vector<aiAnimation*>* animations, std::string path)
{
    this->meshes = meshes;
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    //directory = path.substr(0, path.find_last_of('/'));

    if (scene->HasAnimations()) {
        for (unsigned int i = 0; i < scene->mNumAnimations; i++)
        {
            animations->push_back(scene->mAnimations[i]);
        }
    }
    processNode(scene->mRootNode, scene);
}

void AssimpLoader::processNode(aiNode* node, const aiScene* scene)
{
    // ����� ��� mesh���� ó��(���� �ִٸ�)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes->push_back(processMesh(mesh, scene));
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
            }
        }
    }

    return Mesh::Mesh(vertices, indices/*, textures*/);
}
/*
unsigned int AssimpLoader::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
std::vector<Texture> AssimpLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures_loaded;
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // �ؽ�ó�� �̹� �ҷ��������� �ʴٸ� �ҷ��ɴϴ�.
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory, true);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // �ҷ��� �ؽ�ó�� �����մϴ�.
        }
    }
    return textures;
}
*/