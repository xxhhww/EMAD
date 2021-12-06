#pragma once
#include "../GPUResource.h"

enum class ShaderType : uint8_t{
	VS = 0x00,
	GS = 0x01,
	PS = 0x02,
	Other = 0x03
};

class GPUShader : public GPUResource {
public:
	using ptr = std::shared_ptr<GPUShader>;
public:
	GPUShader(ShaderType sType, const std::string& dir, const std::string& sName, GPUDevice* device);
	~GPUShader() {
		glDeleteShader(mResourceID);
	}

	inline ShaderType GetShaderType() const {
		return mShaderType;
	}

private:
	ShaderType mShaderType;
};