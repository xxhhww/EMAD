#pragma once

#include "DrawCall.h"
#include <map>
#include <vector>

class RenderContext;

// ���Դ������
struct PointLightData {
	PointLightData(glm::vec4 color, glm::vec3 pos, float intensity)
		: LightColor(color)
		, LightPos(pos)
		, LightIntensity(intensity) {}

	glm::vec4 LightColor;
	glm::vec3 LightPos;
	float LightIntensity;
};

// ���������Ҫ���Ƶ�DrawCall
class RenderList {
public:
	// ��pass��Ӧ�������м���drawCall
	inline void AddDrawCall(DrawCallPass pass, DrawCall::ptr drawCall) {
		mDrawCalls[pass].emplace_back(drawCall);
	}

	// ��pass��Ӧ��DrawCall��������
	// Sort DrawCall By Depth(������ȷ��ʾ͸�������͸������?)
	void SortDrawCall(DrawCallPass pass);

	// ִ��pass��Ӧ��DrawCall����
	// ִ��DrawCall��Ҫ�ܶ�һЩ��Ⱦ��Ϣ�����������View��Ϣ������Щ��Ϣ�����RenderContex�У������Ҫһ��ָ������ָ��
	void ExecDrawCall(std::shared_ptr<RenderContext> renderContext, DrawCallPass pass);

	inline void ClearDrawCall() {
		mDrawCalls.clear();
		mPointLights.clear();
	}
public:
	// Ŀǰ����Batch
	std::map<DrawCallPass, std::vector<DrawCall::ptr>> mDrawCalls;
	// ������Ϣ
	std::vector<PointLightData> mPointLights;
};