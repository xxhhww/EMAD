#include "Camera.h"
#include "../../ImGui/imgui.h"

#include <iostream>

#include <DirectXMath.h>
#include <algorithm>

Camera::Camera()
{
	// perspective
	mFovy = glm::radians(45.0f);
	mAspect = 0.0f;
	mNear = 0.1f;
	mFar = 100.0f;
	reset();
	updateVector();
}

void Camera::translate(glm::vec3 translation) noexcept
{
	// 前后移动
	if (translation.x == 0) {
		mPosition += (mFront * translation.z * mTranslationSpeed);
	}
	// 左右移动
	else if (translation.z == 0) {
		mPosition += (mRight * translation.x * mTranslationSpeed);
	}
}

void Camera::rotate(float dx, float dy) noexcept
{
	mYaw += dx * mRotationSpeed;
	mPitch += dy * mRotationSpeed;

	if (mPitch > 89.0f)
		mPitch = 89.0f;
	if (mPitch < -89.0f)
		mPitch = -89.0f;

	if (mYaw > 90.0f)
		mYaw -= 360.0f;
	if (mYaw < -270.0f)
		mYaw += 360.0f;

	updateVector();
}

void Camera::setAspect(float value) noexcept
{
	mAspect = value;
}

glm::mat4 Camera::getView() const noexcept
{
	return glm::lookAt(mPosition, mPosition + mFront, mFixedWorldUp);
}

glm::mat4 Camera::getProjection() const noexcept
{
	return glm::perspective(mFovy, mAspect, mNear, mFar);
}

void Camera::genCtrlGui() noexcept
{
	if (ImGui::Begin("Camera")) {
		ImGui::Text("Location");
		ImGui::SliderFloat("X", &mPosition.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &mPosition.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Z", &mPosition.z, -80.0f, 80.0f, "%.1f");
		ImGui::Text("Rotation");
		ImGui::SliderFloat("Pitch", &mPitch, -89.0f, 89.0f);
		ImGui::SliderFloat("Yaw", &mYaw, -270.0f, 90.0f);
	}
	ImGui::End();

	return;
}

void Camera::reset() noexcept
{
	mPosition = { 0.0f, 0.0f, 3.0f };
	mPitch = 0.0f;
	mYaw = -90.0f;
}

void Camera::updateVector() noexcept
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(front);
	// also re-calculate the Right and Up vector
	mRight = glm::normalize(glm::cross(mFront, mFixedWorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	mUp = glm::normalize(glm::cross(mRight, mFront));
}
