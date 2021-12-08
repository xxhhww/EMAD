#pragma once

#include "ShaderResource.h"
#include <vector>

static const std::string sTexDirectory = "Arts/Texture/"; // 用户纹理资源所在目录

// 用户资产纹理，一般直接从文件中读取
class AssetTexture : public ShaderResource{
public:
	using ptr = std::shared_ptr<AssetTexture>;
public:
	AssetTexture(const std::string& name, GPUDevice* device);

	// 从文件中加载用户资产纹理
	void Update(const std::string& fileName, GPUSampler::ptr sampler, bool isFlip = false);
	// 从文件中加载用户资产纹理
	void Update(std::vector<std::string>& fileNames, GPUSampler::ptr sampler, bool isFlip = false);

	static std::string GenResourceName(const std::vector<std::string>& fileNames);
};