#pragma once
#include "../GPUResource.h"
#include "GPUTexture.h"
#include <vector>

static const std::string sTexDirectory = "Arts/Texture/"; // 用户纹理资源所在目录

// 用户资产纹理，一般直接从文件中读取
class AssetTexture : public GPUResource{
public:
	using ptr = std::shared_ptr<AssetTexture>;
public:
	AssetTexture(const std::string& fileName, GPUSampler::ptr sampler, bool isFlip, GPUDevice* device);
	AssetTexture(std::vector<std::string>& fileNames, GPUSampler::ptr sampler, bool isFlip, GPUDevice* device);

	static std::string GenResourceName(const std::vector<std::string>& fileNames);
private:
	GPUSampler::ptr mSampler;
	GPUTexDesc::ptr mTexDesc;
};