#include "phxpch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace phx {
	static GLenum ShaderTypeToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
		case phx::ShaderDataType::Float: return GL_FLOAT;
		case phx::ShaderDataType::vec2: return GL_FLOAT;
		case phx::ShaderDataType::vec3: return GL_FLOAT;
		case phx::ShaderDataType::vec4: return GL_FLOAT;
		case phx::ShaderDataType::mat3: return GL_FLOAT;
		case phx::ShaderDataType::mat4: return GL_FLOAT;
		case phx::ShaderDataType::Int: return GL_INT;
		case phx::ShaderDataType::int2: return GL_INT;
		case phx::ShaderDataType::int3: return GL_INT;
		case phx::ShaderDataType::int4: return GL_INT;
		case phx::ShaderDataType::Bool: return GL_BOOL;
		}
	}
	OpenGLVertexArray::OpenGLVertexArray()
	{
		PHX_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		PHX_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		PHX_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		PHX_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		PHX_PROFILE_FUNCTION();

		PHX_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderTypeToOpenGLType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		PHX_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}