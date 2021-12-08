#pragma once

#include "DrawCall.h"
#include "RenderList.h"

#include <memory>

class RenderBuffers;
class RenderView;
class GPUContext;

// 渲染上下文，记录渲染所需的信息
class RenderContext : public std::enable_shared_from_this<RenderContext>{
public:
	using ptr = std::shared_ptr<RenderContext>;
public:
	// 初始化函数
	RenderContext();

	// 设置当前的Context
	inline void SetGPUContext(std::shared_ptr<GPUContext> gContext) {
		mGPUContext = gContext;
	}

	// 设置当前的View
	inline void SetRenderView(std::shared_ptr<RenderView> renderView) {
		mRenderView = renderView;
	}

	// 收集各个Object发出的DrawCall
	inline void AddDrawCall(DrawCallPass pass, DrawCall::ptr drawCall) {
		// TODO 判断pass是否存在
		mRenderList->AddDrawCall(pass, drawCall);
	}

	// 执行pass对应的DrawCall
	inline void ExecDrawCall(DrawCallPass pass) {
		mRenderList->ExecDrawCall(shared_from_this(), pass);
	}

	inline void ClearDrawCall() {
		mRenderList->ClearDrawCall();
	}

public:
	std::shared_ptr<GPUContext> mGPUContext;
	// 渲染时，需要使用的缓存
	std::shared_ptr<RenderBuffers> mRenderBuffers;
	// 渲染时，有关View的情况，即如何看待当前需要渲染的物体
	std::shared_ptr<RenderView> mRenderView;
	// 渲染时，需要使用的DrawCalls
	std::shared_ptr<RenderList> mRenderList;
};