#pragma once

#include <Actor/Actor.h>

class Light : public Actor {
public:
protected:
	// ������ɫ
	glm::vec3 mLightColor = { 1.0f, 1.0f, 1.0f };
	// ����ǿ��
	float mIntensity = 150.0f;
};