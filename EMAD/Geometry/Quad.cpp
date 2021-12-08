#include "Quad.h"
#include <Graphics/GPUDevice.h>
#include <Graphics/GPUContext.h>
#include <Graphics/Buffer/DynamicVertex.h>
#include <Graphics/Buffer/VertexBuffer.h>

void Quad::Render(std::shared_ptr<GPUContext> context)
{
	// BindVAO
	context->BindVAO(Quad::GetVB());
	// DrawArrays
	context->DrawArrays(GL_TRIANGLES, 0, 6);
}

std::shared_ptr<VertexBuffer> Quad::GetVB()
{
	bool hasCreated = GPUDevice::Instance()->HasGPUResource("VB_Quad");
	if (hasCreated) {
		return GPUDevice::Instance()->Get<VertexBuffer>("VB_Quad");
	}
	VertexLayout QuadLayout;
	QuadLayout.Attach<AttrType::POSITION_3D>();
	QuadLayout.Attach<AttrType::TEXCOORD_2D>();

	VertexDataArray QuadVertexArray{ QuadLayout, 6u };
	QuadVertexArray[0].Attr<AttrType::POSITION_3D>() = { -1.0f, 1.0f, 0.0f };
	QuadVertexArray[1].Attr<AttrType::POSITION_3D>() = { -1.0f, -1.0f, 0.0f };
	QuadVertexArray[2].Attr<AttrType::POSITION_3D>() = { 1.0f, -1.0f, 0.0f };
	QuadVertexArray[3].Attr<AttrType::POSITION_3D>() = { -1.0f, 1.0f, 0.0f };
	QuadVertexArray[4].Attr<AttrType::POSITION_3D>() = { 1.0f, -1.0f, 0.0f };
	QuadVertexArray[5].Attr<AttrType::POSITION_3D>() = { 1.0f, 1.0f, 0.0f };

	QuadVertexArray[0].Attr<AttrType::TEXCOORD_2D>() = { 0.0f, 1.0f };
	QuadVertexArray[1].Attr<AttrType::TEXCOORD_2D>() = { 0.0f, 0.0f };
	QuadVertexArray[2].Attr<AttrType::TEXCOORD_2D>() = { 1.0f, 0.0f };
	QuadVertexArray[3].Attr<AttrType::TEXCOORD_2D>() = { 0.0f, 1.0f };
	QuadVertexArray[4].Attr<AttrType::TEXCOORD_2D>() = { 1.0f, 0.0f };
	QuadVertexArray[5].Attr<AttrType::TEXCOORD_2D>() = { 1.0f, 1.0f };

	VertexBuffer::ptr QuadVB = GPUDevice::Instance()->Create<VertexBuffer>("VB_Quad");
	QuadVB->Update(QuadVertexArray);

	return QuadVB;
}
