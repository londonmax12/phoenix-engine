#pragma once

#include "Phoenix/Renderer/VertexArray.h"
#include "Phoenix/Renderer/Buffer.h"
#include "Phoenix/Renderer/Shader.h"
#include "Phoenix/Renderer/Camera.h"

#include <glm/glm.hpp>

namespace phx {
	class Mesh
	{
	public:
		Mesh(const std::string& filename);

		struct Vertex
		{
			glm::vec3 Position = {0.0f,0.0f,0.0f};
			glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};
			glm::vec2 TexCoord = {};
			float TexIndex = 0;
			float TilingFactor = 1.0f;
			int EntityID = 1;
		};

		static const int NumAttributes = 5;

		void Render();

		inline const std::string& GetFilePath() const { return m_FilePath; }
		glm::vec3 m_Position = glm::vec3(0,0,0);
	private:
		std::vector<Vertex> m_Vertices;
		std::vector<Vertex> m_TransformedVertices;

		std::vector<uint32_t> m_Indices;

		Ref<Shader> m_MeshShader;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<VertexArray> m_VertexArray;
		Ref<IndexBuffer> m_IndexBuffer;

		std::string m_FilePath;
	};
}