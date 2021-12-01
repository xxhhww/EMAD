#pragma once

#include "DrawCall.h"
#include <map>
#include <vector>

class RenderContext;

// 存放所有需要绘制的DrawCall
class RenderList {
public:
	// 向pass对应的数组中加入drawCall
	inline void AddDrawCall(DrawCallPass pass, DrawCall::ptr drawCall) {
		mDrawCalls[pass].emplace_back(drawCall);
	}

	// 对pass对应的DrawCall进行排序
	// Sort DrawCall By Depth
	void SortDrawCall(DrawCallPass pass);

	// 执行pass对应的DrawCall命令
	// 执行DrawCall需要很多一些渲染信息，比如摄像机View信息，而这些信息存放在RenderContex中，因此需要一个指向它的指针
	void ExecDrawCall(std::shared_ptr<RenderContext> renderContext, DrawCallPass pass);

private:
	// 目前不分Batch
	std::map<DrawCallPass, std::vector<DrawCall::ptr>> mDrawCalls;
};