#pragma once

// 场景纹理枚举类
// 正向渲染的材质渲染时可能需要当前场景已存在的各种纹理信息
enum class SceneTexture {
	SceneDepth = 1,
	BaseColor = 2,
	WorldNormal = 3,
    AmbientOcclusion = 4,
    Metalness = 5,
    Roughness = 6,
    Specular = 7,
};