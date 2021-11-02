#pragma once

#include "../EObject.h"
#include "../../Program/Program.h"

#include <memory>

class Program;
class Drawable : public EObject {
public:
	Drawable(const std::string& name) noexcept
	:mName(name) {}

	virtual ~Drawable() {
		glDeleteVertexArrays(1, &mVAO);
	}

	// 生成控制窗口
	virtual void genCtrlGui() noexcept override {
		if (ImGui::Begin(mName.c_str())) {
			ImGui::Text("Position");
			ImGui::SliderFloat("Pos-X", &mPosition.x, -10.0f, 10.0f, "%.1f");
			ImGui::SliderFloat("Pos-Y", &mPosition.y, -10.0f, 10.0f, "%.1f");
			ImGui::SliderFloat("Pos-Z", &mPosition.z, -10.0f, 10.0f, "%.1f");
			
			ImGui::Text("Rotation");
			ImGui::SliderAngle("Pitch", &mPitch, -90.0f, 90.0f, "%.1f");
			ImGui::SliderAngle("Yaw", &mYaw, -90.0f, 90.0f, "%.1f");
			ImGui::SliderFloat("Roll", &mRoll, -90.0f, 90.0f, "%.1f");
	
			ImGui::Text("Scale");
			ImGui::SliderFloat("Scale-X", &mScaling.x, 0.1f, 3.0f, "%.1f");
			ImGui::SliderFloat("Scale-Y", &mScaling.y, 0.1f, 3.0f, "%.1f");
			ImGui::SliderFloat("Scale-Z", &mScaling.z, 0.1f, 3.0f, "%.1f");
		}
		ImGui::End();
	}

	void setPosition(const glm::vec3& pos) noexcept { mPosition = pos; }
	void setScale(const glm::vec3& scale) noexcept { mScaling = scale; }
	void setRotation(const glm::vec3& rotate) noexcept {
		mPitch = rotate.x;
		mYaw = rotate.y;
		mRoll = rotate.z;
	}

	virtual void draw(std::shared_ptr<Program> program) noexcept {
		program->activate();
		// 绑定顶点着色器的常量缓存
		// bind model trans
		program->setMatrix("model", genModelTrans());

		glBindVertexArray(mVAO);
		glDrawElements(GL_TRIANGLES, mIndexSize, GL_UNSIGNED_INT, 0);
	}

protected:
	// 由控制信息生成模型变换矩阵
	virtual glm::mat4 genModelTrans() const noexcept {
		// 先缩放再旋转后平移
		glm::mat4 modelTrans{ 1.0f };
		glm::mat4 identity{ 1.0f };
		modelTrans *= glm::translate(identity, mPosition);
		modelTrans *= glm::rotate(identity, mPitch, glm::vec3{ 1.0f, 0.0f, 0.0f });
		modelTrans *= glm::rotate(identity, mYaw, glm::vec3{ 0.0f, 1.0f, 0.0f });
		modelTrans *= glm::rotate(identity, glm::radians(mRoll), glm::vec3{ 0.0f, 0.0f, 1.0f });
		modelTrans *= glm::scale(identity, mScaling);

		return modelTrans;
	}
protected:
	// 顶点属性信息
	unsigned int mVAO = 0;
	unsigned int mIndexSize = 0;

	std::string mName;
	// 控制信息
	glm::vec3 mPosition{ 0.0f, 0.0f, 0.0f }; // 位置数据
	glm::vec3 mScaling{ 1.0f, 1.0f, 1.0f };  // 缩放比例
	float mPitch = 0.0f; // 绕X轴旋转
	float mYaw = 0.0f;   // 绕Y轴旋转
	float mRoll = 0.0f;  // 绕Z轴旋转
};