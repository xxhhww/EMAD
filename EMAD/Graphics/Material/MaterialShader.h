#pragma once

#include <memory>

class GPUProgram;
class UniformBuffer;

class MaterialShader {
public:
	using ptr = std::shared_ptr<MaterialShader>;
public:
private:
	std::shared_ptr<GPUProgram> mProgram;
	std::shared_ptr<UniformBuffer> mBuffer;
};