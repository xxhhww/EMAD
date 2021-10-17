#include "Camera.h"

Camera::Camera()
{
	// perspective
	mFovy = glm::radians(45.0f);
	mAspect = 0.0f;
	mNear = 0.1f;
	mFar = 100.0f;
	reset();
}

void Camera::translate(glm::vec3 translation) noexcept
{
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation = glm::rotate(rotation, glm::radians(mPitch), glm::vec3{ 1.0f, 0.0f, 0.0f });
	rotation = glm::rotate(rotation, glm::radians(0.0f), glm::vec3{ 0.0f, 1.0f, 0.0f });

	glm::vec3 trans = rotation * glm::vec4{ translation, 0.0f };

	mPosition += trans;
}

void Camera::rotate(float dx, float dy) noexcept
{
	mYaw += dx * mRotationSpeed;
	mPitch += dy * mRotationSpeed;

	if (mPitch > 89.0f)
		mPitch = 89.0f;
	if (mPitch < -89.0f)
		mPitch = -89.0f;
}

void Camera::setAspect(float value) noexcept
{
	mAspect = value;
}

glm::mat4 Camera::getView() const noexcept
{
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation = glm::rotate(rotation, glm::radians(mPitch), glm::vec3{ 1.0f, 0.0f, 0.0f });
	rotation = glm::rotate(rotation, glm::radians(0.0f), glm::vec3{ 0.0f, 1.0f, 0.0f });

	glm::vec3 lookAtVector = rotation * mFixedFront;
	return glm::lookAt(mPosition, mPosition + lookAtVector, mFixedWorldUp);
}

glm::mat4 Camera::getProjection() const noexcept
{
	return glm::perspective(mFovy, mAspect, mNear, mFar);
}

void Camera::reset() noexcept
{
	mPosition = { 0.0f, 0.0f, 3.0f };
	mPitch = 0.0f;
	mYaw = 0.0f;
}
