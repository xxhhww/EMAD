#pragma once

#include "Light.h"

class SolidSphereMaterial;

class PointLight : public Light {
public:
	using ptr = std::shared_ptr<PointLight>;
public:
	PointLight();

	// ��ǰ֡����Ⱦ�������ύ��Ⱦ����
	virtual void Submit(std::shared_ptr<RenderContext> renderContext) override;

	// PointLight���ƽ���
	virtual void GenCtrlGui() override;
private:
	std::shared_ptr<SolidSphereMaterial> mMaterial;
};