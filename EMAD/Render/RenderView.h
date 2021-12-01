#pragma once

#include <glm/glm.hpp>

// RenderView������ο�����Ҫ��Ⱦ��Object
struct RenderView {
	// �ӵ�
	glm::vec3 MyViewPos;

	// ��ƽ��
	float MyNear;
	// Զƽ��
	float MyFar;
	float MyAspect;
	float MyFovy;

	// ��ͼ�任����
	glm::vec3 MyViewTrans;

	bool isProj = true;
	// ͸�ӱ任����/�����任����
	union {
		glm::vec3 MyProjTrans;
		glm::vec3 MyOrthoTrans;
	};
};