#include "phxpch.h"
#include "Buffer.h"
#include "Phoenix/Application/Core.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace phx {
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: PHX_CORE_ASSERT(false, "Not supported");  return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		PHX_CORE_ASSERT(false, "No render API")
		return nullptr;
	}
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: PHX_CORE_ASSERT(false, "Not supported");  return nullptr;
		case RendererAPI::OpenGL:  return new OpenGLIndexBuffer(indices, size);
		}

		PHX_CORE_ASSERT(false, "No render API")
			return nullptr;
	}
}