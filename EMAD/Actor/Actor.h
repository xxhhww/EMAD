#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RenderContext;

// 基类
class Actor {
public:
	using ptr = std::shared_ptr<Actor>;
public:
	// 提交绘制请求
	virtual void Submit(std::shared_ptr<RenderContext> renderContext) {

	}

	// Actor控制界面
	virtual void GenCtrlGui() {

	}

	// 返回模型变换矩阵
	inline virtual glm::mat4 GenModelTrans() {
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

	// Set方法
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
	float mPitch = 0.0f; // 绕X轴旋转
	float mYaw = 0.0f;   // 绕Y轴旋转
	float mRoll = 0.0f;  // 绕Z轴旋转
};