#include "phxpch.h"

#include "Mesh.h"

#include "RenderCommand.h"
#include "Renderer.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <glad/glad.h>
#include <glm/glm/ext/matrix_transform.hpp>

namespace phx {

	namespace {
		const unsigned int ImportFlags =
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_SortByPType |
			aiProcess_PreTransformVertices |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords |
			aiProcess_OptimizeMeshes |
			aiProcess_Debone |
			aiProcess_ValidateDataStructure;
	}

	Mesh::Mesh(const std::string& filename)
		: m_FilePath(filename)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filename, ImportFlags);

		if (!scene || !scene->HasMeshes())
		{
			PHX_CORE_ERROR("Failed to load mesh: {0}", filename);
			return;
		}

		aiMesh* mesh = scene->mMeshes[0];

		m_Vertices.reserve(mesh->mNumVertices);

		for (size_t i = 0; i < m_Vertices.capacity(); i++)
		{
			Vertex vertex;
			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			//vertex.Color = { mesh->mColors[i]->r, mesh->mColors[i]->g, mesh->mColors[i]->b, mesh->mColors[i]->a };
			
			m_Vertices.push_back(vertex);
		}

		m_VertexBuffer = VertexBuffer::Create(m_Vertices.size() * sizeof(Vertex));
		m_VertexBuffer->SetLayout({
			{ ShaderDataType::vec3, "a_Position"      },
			{ ShaderDataType::vec4, "a_Color"         },
			{ ShaderDataType::vec2, "a_TexCoord"      },
			{ ShaderDataType::Float, "a_TexIndex"     },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Int, "a_EntityID"       }
			});
		m_VertexBuffer->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		//m_Indices.reserve(mesh->mNumFaces * 3);
		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			m_Indices.push_back(mesh->mFaces[i].mIndices[0]);
			m_Indices.push_back(mesh->mFaces[i].mIndices[1]);
			m_Indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size() * 4);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}
}