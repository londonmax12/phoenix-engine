#pragma once

#include <memory>
#include "Phoenix/Renderer/Buffer.h"

namespace phx {
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const phx::Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const phx::Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<phx::Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const phx::Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}
