#include "Surface.h"
#include "../Exception.h"
#include <glad/glad.h>

Texture::ptr TextureMgr::LoadTexture2D(const std::string& dir, const std::string& fileName, bool isFlip)
{
    std::string tag = dir + '/' + fileName;
    // 已经加载过该纹理，直接读取
    if (IsLoaded(tag)) {
        return GetMap()[tag];
    }

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(isFlip);
    unsigned char* data = stbi_load(tag.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
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
    }
    else {
        THROW_INFO_EXCEPTION("Failed to create texture2D");
    }
    stbi_image_free(data);

    GetMap()[tag] = std::make_shared<Texture>(textureID, tag);
    return GetMap()[tag];
}

Texture::ptr TextureMgr::LoadCubeMap(const std::string& dir, std::vector<std::string>& fileNames)
{
    std::string tag;
    tag += dir;
    for (auto& i : fileNames) {
        tag += i;
        i = dir + '/' + i;
    }
    // 已经加载过该纹理，直接读取
    if (IsLoaded(tag)) {
        return GetMap()[tag];
    }

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < fileNames.size(); i++)
    {
        unsigned char* data = stbi_load(fileNames[i].c_str(), &width, &height, &nrChannels, 0);
        if (data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
        }
        else{
            THROW_INFO_EXCEPTION("Failed to create CubeMap");
        }
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    GetMap()[tag] = std::make_shared<Texture>(textureID, tag);
    return GetMap()[tag];
}

bool TextureMgr::IsLoaded(const std::string& tag) noexcept
{
	return (GetMap().find(tag) == GetMap().end()) ? false : true;
}
