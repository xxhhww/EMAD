#include "Material.h"

struct ShaderData {
	glm::mat4 modelTrans;
	glm::mat4 viewTrans;
	glm::mat4 projTrans;
};

Material::Material()
{
}

void Material::Bind(BindInfo& bindInfo)
{
	Shader
	// ͨ��bindInfo�е�DrawCall��Ϣ�����
}
