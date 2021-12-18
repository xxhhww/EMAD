#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RenderContext;

// ����
class Actor {
public:
	using ptr = std::shared_ptr<Actor>;
public:
	// �ύ��������
	virtual void Submit(std::shared_ptr<RenderContext> renderContext) {

	}

	// Actor���ƽ���
	virtual void GenCtrlGui() {

	}

	// ����ģ�ͱ任����
	inline virtual glm::mat4 GenModelTrans() {
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

	// Set����
	inline void SetScale(float x, float y, float z) {
		mScaling = glm::vec3(x, y, z);
	}

	inline glm::vec3 GetScale() const {
		return mScaling;
	}

	inline void SetPosition(float x, float y, float z) {
		mPosition = glm::vec3{ x, y, z };
	}

	inline glm::vec3 GetPosition() const {
		return mPosition;
	}

protected:
	std::string mName = "";
	glm::vec3 mPosition = glm::vec3{ 0.0f, 0.0f, 0.0f };
	glm::vec3 mScaling = glm::vec3{ 1.0f, 1.0f, 1.0f };
	float mPitch = 0.0f; // ��X����ת
	float mYaw = 0.0f;   // ��Y����ת
	float mRoll = 0.0f;  // ��Z����ת
};