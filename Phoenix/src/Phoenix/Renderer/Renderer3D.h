#pragma once

#include "Phoenix/Renderer/OrthographicCamera.h"
#include "Phoenix/Renderer/Camera.h"
#include "Phoenix/Renderer/EditorCamera.h"
#include "Phoenix/Renderer/Texture.h"

#include "Phoenix/Scene/Components.h"

namespace phx {
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const Camera& camera, const glm::mat4& transform);

		static void EndScene();
		static void Flush();

		static void DrawCube(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawCube(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawCube(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawCube(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawCube(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);

		static void DrawRotatedCube(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedCube(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedCube(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedCube(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

		static void UploadInt(std::string name, int i);
		static void UploadFloat(std::string name, float f);
		static void UploadVec3(std::string name, glm::vec3 v);
		static void UploadVec4(std::string name, glm::vec4 v);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t CubeCount = 0;

			uint32_t GetTotalVertexCount() { return CubeCount * 4; }
			uint32_t GetTotalIndexCount() { return CubeCount * 6; }
		};
		static Statistics GetStats();
		static void ResetStats();
	private:
		static void StartBatch();
		static void NextBatch();
	};
}