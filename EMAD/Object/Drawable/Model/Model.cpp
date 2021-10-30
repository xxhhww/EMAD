#include "Model.h"
#include "Node.h"
#include "Mesh.h"

#include "../../../Program/Program.h"
#include "../../../Surface/Surface.h"
#include "../../../Exception.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

Model::Model(const std::string& path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        THROW_INFO_EXCEPTION("Failed to load model");
    }
    mDir = path.substr(0, path.find_last_of('/'));
    // load all meshs
    for (size_t i = 0; i < scene->mNumMeshes; i++) {
        mMeshPtrs.emplace_back(processMesh(scene->mMeshes[i], scene));
    }

    // process node
    mRootNodePtr = std::move(processNode(scene->mRootNode));
}

void Model::draw(std::shared_ptr<Program> program) noexcept
{
    glm::mat4 modelTrans = glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 1.0f });
    mRootNodePtr->draw(program, modelTrans);
}

std::unique_ptr<Node> Model::processNode(aiNode* node) noexcept
{
    std::unique_ptr<Node> curNodePtr = std::make_unique<Node>(node->mName.C_Str(),
        glm::transpose(*reinterpret_cast<glm::mat4*>(&(node->mTransformation))));

    for (size_t i = 0; i < node->mNumMeshes; i++) {
        curNodePtr->addMeshPtr(mMeshPtrs[node->mMeshes[i]]);
    }

    for (size_t i = 0; i < node->mNumChildren; i++) {
        curNodePtr->addChildPtr(processNode(node->mChildren[i]));
    }

    return std::move(curNodePtr);
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene) noexcept
{
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // load vertex attribute
    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 pos, normal;
        glm::vec2 texcoord;

        pos.x = mesh->mVertices[i].x;
        pos.y = mesh->mVertices[i].y;
        pos.z = mesh->mVertices[i].z;

        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0]) {
            texcoord.x = mesh->mTextureCoords[0][i].x;
            texcoord.y = mesh->mTextureCoords[0][i].y;
        }
        else {
            texcoord.x = 0;
            texcoord.y = 0;
        }

        vertices.emplace_back(Vertex{ pos, normal, texcoord });
    }

    // load faces
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.emplace_back(face.mIndices[j]);
    }

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    // 顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // 顶点法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    std::shared_ptr<Mesh> rt = std::make_shared<Mesh>(VAO, indices.size());
    // load materials
    
    if (mesh->mMaterialIndex > 0) {
        // material指向该网格的材质信息
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString diffTexFileName;
        aiString specTexFileName;

        material->GetTexture(aiTextureType_DIFFUSE, 0, &diffTexFileName);
        rt->setDiffuseTex(TextureMgr::LoadTexture2D(diffTexFileName.C_Str(), mDir)->getID(), mDir + '/' + diffTexFileName.C_Str());

        // 有高光贴图
        if (material->GetTexture(aiTextureType_SPECULAR, 0, &specTexFileName) == aiReturn_SUCCESS) {
            rt->setSpecularTex(TextureMgr::LoadTexture2D(specTexFileName.C_Str(), mDir)->getID());
        }
    }
    return rt;
}
