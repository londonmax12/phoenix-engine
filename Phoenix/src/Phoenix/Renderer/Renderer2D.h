#pragma once

#include "Phoenix/Renderer/OrthographicCamera.h"
#include "Phoenix/Renderer/Texture.h"

namespace phx {
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuadFilled(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuadFilled(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuadFilled(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuadFilled(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
	};
}