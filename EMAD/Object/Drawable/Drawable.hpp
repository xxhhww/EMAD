#pragma once

#include "../EObject.h"
#include "../../Shader/ShaderProgram.h"

#include <memory>

class Program;
class Drawable : public EObject {
public:
	virtual ~Drawable() {
		glDeleteVertexArrays(1, &mVAO);
	}

	// ���ɿ��ƴ���
	virtual void genCtrlGui() noexcept override {
		if (ImGui::Begin("Cube")) {
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

	virtual void draw(std::shared_ptr<Program> program) noexcept {
		program->activate();
		// �󶨶�����ɫ���ĳ�������
		// bind model trans
		program->setMatrix("model", genModelTrans());

		glBindVertexArray(mVAO);
		glDrawElements(GL_TRIANGLES, mIndexSize, GL_UNSIGNED_INT, 0);
	}

protected:
	// �ɿ�����Ϣ����ģ�ͱ任����
	virtual glm::mat4 genModelTrans() const noexcept {
		// ����������ת��ƽ��
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
	// ����������Ϣ
	unsigned int mVAO;
	unsigned int mIndexSize;

	// ������Ϣ
	glm::vec3 mPosition{ 0.0f, 0.0f, 0.0f }; // λ������
	glm::vec3 mScaling{ 1.0f, 1.0f, 1.0f };  // ���ű���
	float mPitch = 0.0f; // ��X����ת
	float mYaw = 0.0f;   // ��Y����ת
	float mRoll = 0.0f;  // ��Z����ת
};