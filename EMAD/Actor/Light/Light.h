#pragma once

#include <Actor/Actor.h>

class Light : public Actor {
public:
protected:
	// 光照颜色
	glm::vec3 mLightColor = { 1.0f, 1.0f, 1.0f };
	// 光照强度
	float mIntensity = 150.0f;
};