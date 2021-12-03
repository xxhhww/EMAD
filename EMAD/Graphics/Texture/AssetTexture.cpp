#include "AssetTexture.h"
#include "../../stb_image.h"
#include <iostream>
#include <sstream>

AssetTexture::AssetTexture(const std::string& fileName, GPUSampler::ptr sampler, bool isFlip, GPUDevice* device)
	:ShaderResource(fileName, GPUResource::ResourceType::Texture, device){
    mSampler = sampler;

	glGenTextures(1, &mResourceID);

    std::string tag = sTexDirectory + fileName;
    // load data
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(isFlip);
    unsigned char* data = stbi_load(tag.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RGBA;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, mResourceID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        // Record Texture Description Information
        mTexDesc = GPUTexDesc::Gen2D(0, format, width, height, format, GL_UNSIGNED_BYTE);
        // Gen MipMap
        glGenerateMipmap(GL_TEXTURE_2D);
        // Set Filter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mSampler->WRAPS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mSampler->WRAPT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mSampler->MinFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mSampler->MagFilter);
        glBindTexture(GL_TEXTURE_2D, 0);
        // free data
        stbi_image_free(data);
    }
    // Error
    else {
        std::cout << "Failed to create texture2D" << std::endl;
    }
}

AssetTexture::AssetTexture(std::vector<std::string>& fileNames, GPUSampler::ptr sampler, bool isFlip, GPUDevice* device)
	:ShaderResource(GenResourceName(fileNames), GPUResource::ResourceType::Texture, device){
    mSampler = sampler;

    for (auto& i : fileNames) {
        i = sTexDirectory + i;
    }

    glGenTextures(1, &mResourceID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mResourceID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < fileNames.size(); i++){
        // load data
        unsigned char* data = stbi_load(fileNames[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = GL_RGB;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            // Record Texture Description Information
            if(mTexDesc == nullptr)
                mTexDesc = GPUTexDesc::Gen2D(0, format, width, height, format, GL_UNSIGNED_BYTE);
            // free data
            stbi_image_free(data);
        }
        else {
            std::cout << "Failed to create CubeTexture" << std::endl;
        }
    }
    // Set Filter
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, mSampler->WRAPS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, mSampler->WRAPT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, mSampler->WRAPR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, mSampler->MinFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mSampler->MagFilter);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

std::string AssetTexture::GenResourceName(const std::vector<std::string>& fileNames)
{
    std::stringstream ss;
    for (const auto& i : fileNames) {
        ss << i << "|";
    }
    return ss.str();
}
