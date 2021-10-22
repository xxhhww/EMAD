#include "PointLight.h"
#include "../../Shader/ShaderProgram.h"
#include "../Geometry/Sphere.h"

PointLight::PointLight()
{
	// create shader program
	mProgram = std::make_shared<Program>("Shader/PointLight_vs.vert", "Shader/PointLight_fs.frag");

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

void PointLight::draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	mProgram->activate();
	// 绑定顶点着色器的常量缓存
	// bind model trans
	mProgram->setMatrix("model", glm::value_ptr(model));
	// bind view trans
	mProgram->setMatrix("view", glm::value_ptr(view));
	// bind projection trans
	mProgram->setMatrix("projection", glm::value_ptr(projection));

	glBindVertexArray(mVAO);

	glDrawElements(GL_TRIANGLES, mIndexSize, GL_UNSIGNED_INT, 0);
}
