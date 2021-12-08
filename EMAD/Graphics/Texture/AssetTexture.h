#pragma once

#include "ShaderResource.h"
#include <vector>

static const std::string sTexDirectory = "Arts/Texture/"; // �û�������Դ����Ŀ¼

// �û��ʲ�����һ��ֱ�Ӵ��ļ��ж�ȡ
class AssetTexture : public ShaderResource{
public:
	using ptr = std::shared_ptr<AssetTexture>;
public:
	AssetTexture(const std::string& name, GPUDevice* device);

	// ���ļ��м����û��ʲ�����
	void Update(const std::string& fileName, GPUSampler::ptr sampler, bool isFlip = false);
	// ���ļ��м����û��ʲ�����
	void Update(std::vector<std::string>& fileNames, GPUSampler::ptr sampler, bool isFlip = false);

	static std::string GenResourceName(const std::vector<std::string>& fileNames);
};