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
	// 生成新的着色器代码并编译
	virtual void Init() override;
	virtual void Bind(BindInfo& bindinfo) override;

	// Set方法
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

	// 生成材质控制界面
	virtual void GenCtrlGui() override;

public:
	// 根据是否使用贴图生成对应的着色器代码
	std::string GenShaderCode();

private:
	// 是否发生修改
	bool mIsDirty = false;

	// 用户控制界面的输入情况
	bool mHasBaseColorTex = false;
	bool mHasNormalTex = false;
	bool mHasMetallicTex = false;
	bool mHasRoughnessTex = false;
	bool mHasAoTex = false;
	
	// 没有贴图时，使用的数据
	glm::vec3 mBaseColor = glm::vec3{ 1.0f };
	float mMetallic = 0.1f;
	float mRoughness = 0.0f;
	float mAo = 1.0f;

	// 用户纹理资产
	std::shared_ptr<AssetTexture> mBaseColorTex = nullptr;
	std::shared_ptr<AssetTexture> mNormalTex = nullptr;
	std::shared_ptr<AssetTexture> mMetallicTex = nullptr;
	std::shared_ptr<AssetTexture> mRoughnessTex = nullptr;
	std::shared_ptr<AssetTexture> mAoTex = nullptr;

	//
	std::shared_ptr<GPUProgram> mShaderProgram = nullptr;
	std::shared_ptr<UniformBuffer> mShaderBuffer = nullptr;
};