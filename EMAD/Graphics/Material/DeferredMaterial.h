#pragma once

#include "Material.h"

#include <string>

#include <glm/glm.hpp>

class GPUProgram;
class AssetTexture;
class UniformBuffer;

class DeferredMaterial : public Material {
public:
	using ptr = std::shared_ptr<DeferredMaterial>;
public:
	// �����µ���ɫ�����벢����
	virtual void Init() override;
	virtual void Bind(BindInfo& bindinfo) override;

	// Set����
	inline void SetBaseColorTex(std::shared_ptr<AssetTexture> tex) {
		mBaseColorTex = tex;
	}

	inline void SetNormalTex(std::shared_ptr<AssetTexture> tex) {
		mNormalTex = tex;
	}

	inline void SetMetallicTex(std::shared_ptr<AssetTexture> tex) {
		mMetallicTex = tex;
	}

	inline void SetRoughnessTex(std::shared_ptr<AssetTexture> tex) {
		mRoughnessTex = tex;
	}

	inline void SetAoTex(std::shared_ptr<AssetTexture> tex) {
		mAoTex = tex;
	}

	// ���ɲ��ʿ��ƽ���
	virtual void GenCtrlGui() override;

public:
	// �����Ƿ�ʹ����ͼ���ɶ�Ӧ����ɫ������
	std::string GenShaderCode();

private:
	// �Ƿ����޸�
	bool mIsDirty = false;

	// �û����ƽ�����������
	bool mHasBaseColorTex = false;
	bool mHasNormalTex = false;
	bool mHasMetallicTex = false;
	bool mHasRoughnessTex = false;
	bool mHasAoTex = false;
	
	// û����ͼʱ��ʹ�õ�����
	glm::vec3 mBaseColor = glm::vec3{ 1.0f };
	float mMetallic = 0.1f;
	float mRoughness = 0.0f;
	float mAo = 1.0f;

	// �û������ʲ�
	std::shared_ptr<AssetTexture> mBaseColorTex = nullptr;
	std::shared_ptr<AssetTexture> mNormalTex = nullptr;
	std::shared_ptr<AssetTexture> mMetallicTex = nullptr;
	std::shared_ptr<AssetTexture> mRoughnessTex = nullptr;
	std::shared_ptr<AssetTexture> mAoTex = nullptr;

	//
	std::shared_ptr<GPUProgram> mShaderProgram = nullptr;
	std::shared_ptr<UniformBuffer> mShaderBuffer = nullptr;
};