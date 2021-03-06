#pragma once
#include "../EObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera : public EObject{
public:
	Camera();

	void translate(glm::vec3 translation) noexcept;
	void rotate(float dx, float dy) noexcept;

	void setAspect(float value) noexcept;
	glm::vec3 getPosition() const noexcept;
	glm::mat4 getView() const noexcept;
	glm::mat4 getProjection() const noexcept;

	virtual void genCtrlGui() noexcept override;
private:
	void reset() noexcept;
	void updateVector() noexcept;
private:
	float mFovy;
	float mAspect;
	float mNear;
	float mFar;

	glm::vec3 mPosition; // ????λ??
	float mPitch; //??????
	float mYaw; //ƫ????

	glm::vec3 mFront = { 0.0f, 0.0f, -1.0f };
	glm::vec3 mRight = { 1.0f, 0.0f, 0.0f };
	glm::vec3 mUp = { 0.0f, 1.0f ,0.0f };

	const float mTranslationSpeed = 10.0f;
	const float mRotationSpeed = 0.04f;
	const glm::vec3 mFixedWorldUp = { 0.0f, 1.0f, 0.0f };
};