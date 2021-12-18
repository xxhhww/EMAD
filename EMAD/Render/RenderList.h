#pragma once

#include "DrawCall.h"
#include <map>
#include <vector>

class RenderContext;

// 点光源的数据
struct PointLightData {
	PointLightData(glm::vec4 color, glm::vec3 pos, float intensity)
		: LightColor(color)
		, LightPos(pos)
		, LightIntensity(intensity) {}

	glm::vec4 LightColor;
	glm::vec3 LightPos;
	float LightIntensity;
};

// 存放所有需要绘制的DrawCall
class RenderList {
public:
	// 向pass对应的数组中加入drawCall
	inline void AddDrawCall(DrawCallPass pass, DrawCall::ptr drawCall) {
		mDrawCalls[pass].emplace_back(drawCall);
	}

	// 对pass对应的DrawCall进行排序
	// Sort DrawCall By Depth(用于正确显示透明物体的透明特性?)
	void SortDrawCall(DrawCallPass pass);

	// 执行pass对应的DrawCall命令
	// 执行DrawCall需要很多一些渲染信息，比如摄像机View信息，而这些信息存放在RenderContex中，因此需要一个指向它的指针
	void ExecDrawCall(std::shared_ptr<RenderContext> renderContext, DrawCallPass pass);

	inline void ClearDrawCall() {
		mDrawCalls.clear();
		mPointLights.clear();
	}
public:
	// 目前不分Batch
	std::map<DrawCallPass, std::vector<DrawCall::ptr>> mDrawCalls;
	// 光照信息
	std::vector<PointLightData> mPointLights;
};