#include "GPUDevice.h"

#include "GPUContext.h"
#include "Texture/AssetTexture.h"
#include "Texture/GPUTexture.h"
#include "Shader/GPUProgram.h"
#include "Buffer/VertexBuffer.h"
#include "Buffer/UniformBuffer.h"
#include "Buffer/FrameBuffer.h"

GPUDevice::GPUDevice()
{
	mMainContext = std::make_shared<GPUContext>(this);
}
