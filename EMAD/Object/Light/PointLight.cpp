#include "PointLight.h"
#include "../../Program/Program.h"
#include "../Geometry/Sphere.h"

PointLight::PointLight(const std::string& name)
	:Drawable(name)
{
	mPosition = { 0.0f, 0.0f, -1.0f };
	mScaling = { 0.1f, 0.1f, 0.1f };

	// vertices
	auto vertexData = Sphere::Make();
	mIndexSize = vertexData.second.size();

	// 初始化VAO
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind VAO
	glBindVertexArray(mVAO);
	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexData.first.size() * sizeof(float), vertexData.first.data(), GL_STATIC_DRAW);
	// bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexData.second.size() * sizeof(unsigned int), vertexData.second.data(), GL_STATIC_DRAW);
	// set vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// unbind VBO
	// 已经通过glVertexAttributePointer将VBO中的顶点属性数据绑定到了VAO中，因此可以取消绑定
	// 但是不能对EBO这样做
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void PointLight::genCtrlGui() noexcept
{
	if (ImGui::Begin(mName.c_str())) {
		ImGui::Text("Position");
		ImGui::SliderFloat("Pos-X", &mPosition.x, -10.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("Pos-Y", &mPosition.y, -10.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("Pos-Z", &mPosition.z, -10.0f, 10.0f, "%.1f");

		ImGui::Text("Light");
		ImGui::ColorEdit3("Diffuse", &mColor.x);
		ImGui::SliderFloat("Ambient", &mAmbient, 0.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("Specular", &mSpecular, 0.0f, 1.0f, "%.1f");
	}
	ImGui::End();
}

void PointLight::draw(std::shared_ptr<Program> program) noexcept
{
	program->activate();
	// 绑定顶点着色器的常量缓存
	// bind model trans
	program->setMatrix("model", genModelTrans());

	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mIndexSize, GL_UNSIGNED_INT, 0);
}

