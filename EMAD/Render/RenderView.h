#pragma once

#include <glm/glm.hpp>

// RenderView������ο�����Ҫ��Ⱦ��Object
struct RenderView {
	using ptr = std::shared_ptr<RenderView>;

	// �ӵ�
	glm::vec3 MyViewPos;

	// ��ƽ��
	float MyNear;
	// Զƽ��
	float MyFar;
	float MyAspect;
	float MyFovy;

	// ��ͼ�任����
	glm::mat4 MyViewTrans;

	bool isProj = true;
	// ͸�ӱ任����/�����任����
	union {
		glm::mat4 MyProjTrans;
		glm::mat4 MyOrthoTrans;
	};
};