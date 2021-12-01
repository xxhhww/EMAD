#pragma once

#include "../Texture/AssetTexture.h"

#include <string>
#include <glm/glm.hpp>

// 材质参数类型
enum class MaterialParameterType
{
    // The invalid type.
    Invalid = 0,
    // The bool.
    Bool = 1,
    // The integer.
    Integer = 2,
    // The float.
    Float = 3,
    // The vector2
    Vector2 = 4,
    // The vector3.
    Vector3 = 5,
    // The vector4.
    Vector4 = 6,
    // The matrix.
    Matrix = 7,
    // The texture.
    Texture = 8,
    // The cube texture.
    CubeTexture = 9,
};

// 材质参数
// 用来控制材质的视觉表现效果
struct MaterialParameter {
    MaterialParameterType Type = MaterialParameterType::Invalid; // 参数类型
    std::string Name; // 参数名称
    // 参数数据
    union
    {
        bool AsBool;
        int32_t AsInteger;
        float AsFloat;
        glm::vec2 AsVector2;
        glm::vec3 AsVector3;
        glm::vec4 AsVector4;
        glm::mat4 AsMatrix;
    };
    AssetTexture::ptr AsTexture;
    unsigned int RegisterIndex; // 对应的纹理单元
};

class MaterialParas {
public:
private:
    std::vector<MaterialParameter> mParas;
};