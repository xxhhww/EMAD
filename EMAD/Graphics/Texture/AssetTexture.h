#pragma once
#include "../GPUResource.h"
#include "GPUTexture.h"
#include <vector>

static const std::string sTexDirectory = "Arts/Texture/"; // �û�������Դ����Ŀ¼

// �û��ʲ�����һ��ֱ�Ӵ��ļ��ж�ȡ
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