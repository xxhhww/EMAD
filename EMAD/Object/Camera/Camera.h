#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	Camera();

	void translate(glm::vec3 translation) noexcept;
	void rotate(float dx, float dy) noexcept;

	void setAspect(float value) noexcept;
	glm::mat4 getView() const noexcept;
	glm::mat4 getProjection() const noexcept;
private:
	void reset() noexcept;
private:
	float mFovy;
	float mAspect;
	float mNear;
	float mFar;

	glm::vec3 mPosition; // Ïà»úÎ»ÖÃ
	float mPitch; //¸©Ñö½Ç
	float mYaw; //Æ«º½½Ç

	const float mTranslationSpeed = 12.0f;
	const float mRotationSpeed = 0.05f;
	const glm::vec4 mFixedFront = { 0.0f, 0.0f, -1.0f, 0.0f };
	const glm::vec3 mFixedWorldUp = { 0.0f, 1.0f, 0.0f };
};