#pragma once

#include "Light.h"

class SolidSphereMaterial;

class PointLight : public Light {
public:
	using ptr = std::shared_ptr<PointLight>;
public:
	PointLight();

	// 向当前帧的渲染上下文提交渲染数据
	virtual void Submit(std::shared_ptr<RenderContext> renderContext) override;

	// PointLight控制界面
	virtual void GenCtrlGui() override;
private:
	std::shared_ptr<SolidSphereMaterial> mMaterial;
};