#pragma once

#include "RendererAPI.h"

namespace phx {
	class RenderCommand {
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}
		inline static void ClearColor(const glm::vec4& color)
		{
			s_RendererAPI->ClearColor(color);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}