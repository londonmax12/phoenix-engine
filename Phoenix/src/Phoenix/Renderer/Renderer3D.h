#pragma once

#include "Phoenix/Renderer/OrthographicCamera.h"
#include "Phoenix/Renderer/Camera.h"
#include "Phoenix/Renderer/EditorCamera.h"
#include "Phoenix/Renderer/Texture.h"
#include "Phoenix/Renderer/Shader.h"

#include "Phoenix/Scene/Components.h"

namespace phx {
	struct Vertex3D
	{
		glm::vec3 Position = { 0.0f,0.0f,0.0f };
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec2 TexCoord = {};
		float TexIndex = 0;
		float TilingFactor = 1.0f;
		int EntityID = 1;
	};
	struct SceneData
	{
		glm::mat4 ViewProjectionMatrix;
	};
	static SceneData m_SceneData;
	static Ref<Shader> m_MeshShader;

	class Renderer3D
	{
	public:
		static void Init();

		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const Camera& camera, const glm::mat4& transform);

		static void EndScene();

		static void SubmitMesh(Mesh& mesh, glm::mat4 transform, int entityID);
	private:

	};
}