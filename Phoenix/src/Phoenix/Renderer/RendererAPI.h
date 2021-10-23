#pragma once
#include "phxpch.h"
#include "VertexArray.h"
#include <../vendor/glm/glm/glm.hpp>

namespace phx {
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void ClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		inline static API getAPI() { return s_API; }
	private:
		static API s_API;
	};
}