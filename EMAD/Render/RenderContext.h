#pragma once

#include "DrawCall.h"
#include "RenderList.h"

#include <memory>

class RenderBuffers;
class RenderView;
class GPUContext;

// ��Ⱦ�����ģ���¼��Ⱦ�������Ϣ
class RenderContext : public std::enable_shared_from_this<RenderContext>{
public:
	using ptr = std::shared_ptr<RenderContext>;
public:
	// ��ʼ������
	RenderContext();

	// ���õ�ǰ��Context
	inline void SetGPUContext(std::shared_ptr<GPUContext> gContext) {
		mGPUContext = gContext;
	}

	// ���õ�ǰ��View
	inline void SetRenderView(std::shared_ptr<RenderView> renderView) {
		mRenderView = renderView;
	}

	// �ռ�����Object������DrawCall
	inline void AddDrawCall(DrawCallPass pass, DrawCall::ptr drawCall) {
		// TODO �ж�pass�Ƿ����
		mRenderList->AddDrawCall(pass, drawCall);
	}

	// ִ��pass��Ӧ��DrawCall
	inline void ExecDrawCall(DrawCallPass pass) {
		mRenderList->ExecDrawCall(shared_from_this(), pass);
	}

	inline void ClearDrawCall() {
		mRenderList->ClearDrawCall();
	}

public:
	std::shared_ptr<GPUContext> mGPUContext;
	// ��Ⱦʱ����Ҫʹ�õĻ���
	std::shared_ptr<RenderBuffers> mRenderBuffers;
	// ��Ⱦʱ���й�View�����������ο�����ǰ��Ҫ��Ⱦ������
	std::shared_ptr<RenderView> mRenderView;
	// ��Ⱦʱ����Ҫʹ�õ�DrawCalls
	std::shared_ptr<RenderList> mRenderList;
};