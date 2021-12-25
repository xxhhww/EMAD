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
	// ��ʼ��/����
	virtual void Init() = 0;
	// �����ʵ����ݰ󶨵�GPUContext
	virtual void Bind(BindInfo& bindinfo) = 0;
	// ���ɲ��ʿ��ƽ���
	virtual void GenCtrlGui() {

	}
};