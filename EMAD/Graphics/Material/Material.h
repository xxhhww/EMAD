#pragma once

#include <memory>
#include "MaterialShader.h"

class RenderContext;
class DrawCall;

class Material {
public:
	using ptr = std::shared_ptr<Material>;

	struct BindInfo {
		std::shared_ptr<RenderContext> MyRenderContext;
		std::shared_ptr<DrawCall> MyDrawCall;
	};

public:
	// 初始化/更新
	virtual void Init() = 0;
	// 将材质的数据绑定到GPUContext
	virtual void Bind(BindInfo& bindinfo) = 0;
	// 生成材质控制界面
	virtual void GenCtrlGui() {

	}
};