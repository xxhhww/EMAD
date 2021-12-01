#pragma once

#include "DrawCall.h"
#include <map>
#include <vector>

class RenderContext;

// ���������Ҫ���Ƶ�DrawCall
class RenderList {
public:
	// ��pass��Ӧ�������м���drawCall
	inline void AddDrawCall(DrawCallPass pass, DrawCall::ptr drawCall) {
		mDrawCalls[pass].emplace_back(drawCall);
	}

	// ��pass��Ӧ��DrawCall��������
	// Sort DrawCall By Depth
	void SortDrawCall(DrawCallPass pass);

	// ִ��pass��Ӧ��DrawCall����
	// ִ��DrawCall��Ҫ�ܶ�һЩ��Ⱦ��Ϣ�����������View��Ϣ������Щ��Ϣ�����RenderContex�У������Ҫһ��ָ������ָ��
	void ExecDrawCall(std::shared_ptr<RenderContext> renderContext, DrawCallPass pass);

private:
	// Ŀǰ����Batch
	std::map<DrawCallPass, std::vector<DrawCall::ptr>> mDrawCalls;
};