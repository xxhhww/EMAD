#pragma once

#include "DrawCall.h"
#include <memory>

class RenderBuffers;
class RenderView;
class RenderList;
class GPUContext;

// ��Ⱦ�����ģ���¼��Ⱦ�������Ϣ
class RenderContext : public std::enable_shared_from_this<RenderContext>{
public:
	// ��ʼ������
	RenderContext();

	// �ռ�����Object������DrawCall
	void AddDrawCall(DrawCallPass pass, DrawCall::ptr drawCall);

	// ���õ�ǰ��Context
	inline void SetGPUContext(std::shared_ptr<GPUContext> gContext) {
		mGPUContext = gContext;
	}

	// ���õ�ǰ��View
	inline void SetRenderView(std::shared_ptr<RenderView> renderView) {
		mRenderView = renderView;
	}

	// ִ��pass��Ӧ��DrawCall
	void ExecDrawCall(DrawCallPass pass);

public:
	std::shared_ptr<GPUContext> mGPUContext;
	// ��Ⱦʱ����Ҫʹ�õĻ���
	std::shared_ptr<RenderBuffers> mRenderBuffers;
	// ��Ⱦʱ���й�View�����������ο�����ǰ��Ҫ��Ⱦ������
	std::shared_ptr<RenderView> mRenderView;
	// ��Ⱦʱ����Ҫʹ�õ�DrawCalls
	std::shared_ptr<RenderList> mRenderList;
};