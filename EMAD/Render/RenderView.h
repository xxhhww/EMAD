#pragma once

#include <glm/glm.hpp>

// RenderView描述如何看待需要渲染的Object
struct RenderView {
	using ptr = std::shared_ptr<RenderView>;

	// 视点
	glm::vec3 MyViewPos;

	// 近平面
	float MyNear;
	// 远平面
	float MyFar;
	float MyAspect;
	float MyFovy;

	// 视图变换矩阵
	glm::mat4 MyViewTrans;

	bool isProj = true;
	// 透视变换矩阵/正交变换矩阵
	union {
		glm::mat4 MyProjTrans;
		glm::mat4 MyOrthoTrans;
	};
};