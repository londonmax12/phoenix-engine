#pragma once

#include "Phoenix/Renderer/RendererAPI.h"

namespace phx {
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void ClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;

	};
}