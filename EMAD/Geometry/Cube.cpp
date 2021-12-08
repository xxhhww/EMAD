#include "Cube.h"
#include <Graphics/GPUDevice.h>
#include <Graphics/GPUContext.h>
#include <Graphics/Buffer/DynamicVertex.h>
#include <Graphics/Buffer/VertexBuffer.h>

std::shared_ptr<VertexBuffer> Cube::GetVB()
{
	bool hasCreated = GPUDevice::Instance()->HasGPUResource("VB_Cube");
	if (hasCreated) {
		return GPUDevice::Instance()->Get<VertexBuffer>("VB_Cube");
	}
	VertexLayout CubeLayout;
	CubeLayout.Attach<AttrType::POSITION_3D>();
	CubeLayout.Attach<AttrType::TEXCOORD_2D>();

	VertexDataArray CubeVertexArray{ CubeLayout, 24u };
	constexpr float side = 1.0f / 2.0f;

	CubeVertexArray[0].Attr<AttrType::POSITION_3D>() = { -side,-side,-side };// 0 near side
	CubeVertexArray[1].Attr<AttrType::POSITION_3D>() = { side,-side,-side };// 1
	CubeVertexArray[2].Attr<AttrType::POSITION_3D>() = { -side,side,-side };// 2
	CubeVertexArray[3].Attr<AttrType::POSITION_3D>() = { side,side,-side };// 3
	CubeVertexArray[4].Attr<AttrType::POSITION_3D>() = { -side,-side,side };// 4 far side
	CubeVertexArray[5].Attr<AttrType::POSITION_3D>() = { side,-side,side };// 5
	CubeVertexArray[6].Attr<AttrType::POSITION_3D>() = { -side,side,side };// 6
	CubeVertexArray[7].Attr<AttrType::POSITION_3D>() = { side,side,side };// 7
	CubeVertexArray[8].Attr<AttrType::POSITION_3D>() = { -side,-side,-side };// 8 left side
	CubeVertexArray[9].Attr<AttrType::POSITION_3D>() = { -side,side,-side };// 9
	CubeVertexArray[10].Attr<AttrType::POSITION_3D>() = { -side,-side,side };// 10
	CubeVertexArray[11].Attr<AttrType::POSITION_3D>() = { -side,side,side };// 11
	CubeVertexArray[12].Attr<AttrType::POSITION_3D>() = { side,-side,-side };// 12 right side
	CubeVertexArray[13].Attr<AttrType::POSITION_3D>() = { side,side,-side };// 13
	CubeVertexArray[14].Attr<AttrType::POSITION_3D>() = { side,-side,side };// 14
	CubeVertexArray[15].Attr<AttrType::POSITION_3D>() = { side,side,side };// 15
	CubeVertexArray[16].Attr<AttrType::POSITION_3D>() = { -side,-side,-side };// 16 bottom side
	CubeVertexArray[17].Attr<AttrType::POSITION_3D>() = { side,-side,-side };// 17
	CubeVertexArray[18].Attr<AttrType::POSITION_3D>() = { -side,-side,side };// 18
	CubeVertexArray[19].Attr<AttrType::POSITION_3D>() = { side,-side,side };// 19
	CubeVertexArray[20].Attr<AttrType::POSITION_3D>() = { -side,side,-side };// 20 top side
	CubeVertexArray[21].Attr<AttrType::POSITION_3D>() = { side,side,-side };// 21
	CubeVertexArray[22].Attr<AttrType::POSITION_3D>() = { -side,side,side };// 22
	CubeVertexArray[23].Attr<AttrType::POSITION_3D>() = { side,side,side };// 23

	CubeVertexArray[0].Attr<AttrType::TEXCOORD_2D>() = { 0.0f,0.0f };
	CubeVertexArray[1].Attr<AttrType::TEXCOORD_2D>() = { 1.0f,0.0f };
	CubeVertexArray[2].Attr<AttrType::TEXCOORD_2D>() = { 0.0f,1.0f };
	CubeVertexArray[3].Attr<AttrType::TEXCOORD_2D>() = { 1.0f,1.0f };
	CubeVertexArray[4].Attr<AttrType::TEXCOORD_2D>() = { 0.0f,0.0f };
	CubeVertexArray[5].Attr<AttrType::TEXCOORD_2D>() = { 1.0f,0.0f };
	CubeVertexArray[6].Attr<AttrType::TEXCOORD_2D>() = { 0.0f,1.0f };
	CubeVertexArray[7].Attr<AttrType::TEXCOORD_2D>() = { 1.0f,1.0f };
	CubeVertexArray[8].Attr<AttrType::TEXCOORD_2D>() = { 0.0f,0.0f };
	CubeVertexArray[9].Attr<AttrType::TEXCOORD_2D>() = { 1.0f,0.0f };
	CubeVertexArray[10].Attr<AttrType::TEXCOORD_2D>() = { 0.0f,1.0f };
	CubeVertexArray[11].Attr<AttrType::TEXCOORD_2D>() = { 1.0f,1.0f };
	CubeVertexArray[12].Attr<AttrType::TEXCOORD_2D>() = { 0.0f,0.0f };
	CubeVertexArray[13].Attr<AttrType::TEXCOORD_2D>() = { 1.0f,0.0f };
	CubeVertexArray[14].Attr<AttrType::TEXCOORD_2D>() = { 0.0f,1.0f };
	CubeVertexArray[15].Attr<AttrType::TEXCOORD_2D>() = { 1.0f,1.0f };
	CubeVertexArray[16].Attr<AttrType::TEXCOORD_2D>() = { 0.0f,0.0f };
	CubeVertexArray[17].Attr<AttrType::TEXCOORD_2D>() = { 1.0f,0.0f };
	CubeVertexArray[18].Attr<AttrType::TEXCOORD_2D>() = { 0.0f,1.0f };
	CubeVertexArray[19].Attr<AttrType::TEXCOORD_2D>() = { 1.0f,1.0f };
	CubeVertexArray[20].Attr<AttrType::TEXCOORD_2D>() = { 0.0f,0.0f };
	CubeVertexArray[21].Attr<AttrType::TEXCOORD_2D>() = { 1.0f,0.0f };
	CubeVertexArray[22].Attr<AttrType::TEXCOORD_2D>() = { 0.0f,1.0f };
	CubeVertexArray[23].Attr<AttrType::TEXCOORD_2D>() = { 1.0f,1.0f };

	std::vector<unsigned int> indices = {
		0,2, 1,    2,3,1,
		4,5, 7,    4,7,6,
		8,10, 9,  10,11,9,
		12,13,15, 12,15,14,
		16,17,18, 18,17,19,
		20,23,21, 20,22,23
	};

	VertexBuffer::ptr CubeVB = GPUDevice::Instance()->Create<VertexBuffer>("VB_Cube");
	CubeVB->Update(CubeVertexArray, indices);

	return CubeVB;
}
