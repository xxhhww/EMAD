#pragma once

#include "Material.h"

class GPUProgram;
class AssetTexture;
class UniformBuffer;

class DeferredMaterial : public Material {
public:
	using ptr = std::shared_ptr<DeferredMaterial>;
public:
	virtual void Init() override;
	virtual void Bind(BindInfo& bindinfo) override;

	// Set方法
	inline void SetBaseColorTex(std::shared_ptr<AssetTexture> tex) {
		mBaseColorTex = tex;
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
private:
	std::shared_ptr<GPUProgram> mShaderProgram;
	std::shared_ptr<UniformBuffer> mUniformBuffer;
	// 用户纹理资产
	std::shared_ptr<AssetTexture> mBaseColorTex;
	std::shared_ptr<AssetTexture> mMetallicTex;
	std::shared_ptr<AssetTexture> mRoughnessTex;
	std::shared_ptr<AssetTexture> mAoTex;
};