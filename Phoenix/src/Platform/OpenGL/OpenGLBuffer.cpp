#include "phxpch.h"
#include "Phoenix/Renderer/Buffer.h"
#include "OpenGLBuffer.h"
#include "glad/glad.h"

namespace phx {
	//--------------------------
	// VERTEX BUFFER DEFINITIONS
	//--------------------------

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		PHX_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		PHX_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		PHX_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		PHX_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLVertexBuffer::Unbind() const
	{
		PHX_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	//--------------------------
	// INDEX BUFFER DEFINITIONS
	//--------------------------

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		PHX_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(Indice* indices, uint32_t count)
	{

	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		PHX_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		PHX_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLIndexBuffer::Unbind() const
	{
		PHX_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}