#include "phxpch.h"
#include "Renderer3D.h"

#include "Phoenix/Renderer/VertexArray.h"
#include "Phoenix/Renderer/Shader.h"
#include "Phoenix/Renderer/UniformBuffer.h"
#include "Phoenix/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace phx
{
	struct CubeVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// Editor-only
		int EntityID;
	};

	struct Renderer3DData
	{
		static const uint32_t MaxCubes = 20000;
		static const uint32_t MaxVertices = MaxCubes * 8;
		static const uint32_t MaxIndices = MaxCubes * 36;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> CubeVertexArray;
		Ref<VertexBuffer> CubeVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t CubeIndexCount = 0;
		CubeVertex* CubeVertexBufferBase = nullptr;
		CubeVertex* CubeVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 CubeVertexPositions[24];

		Renderer3D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		PHX_PROFILE_FUNCTION();

		s_Data.CubeVertexArray = VertexArray::Create();

		s_Data.CubeVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CubeVertex));
		s_Data.CubeVertexBuffer->SetLayout({
			{ ShaderDataType::vec3, "a_Position" },
			{ ShaderDataType::vec4, "a_Color" },
			{ ShaderDataType::vec2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Int, "a_EntityID" }
			});
		s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);

		s_Data.CubeVertexBufferBase = new CubeVertex[s_Data.MaxVertices];

		uint32_t* cubeIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			cubeIndices[i + 0] = offset + 0;
			cubeIndices[i + 1] = offset + 1;
			cubeIndices[i + 2] = offset + 2;

			cubeIndices[i + 3] = offset + 2;
			cubeIndices[i + 4] = offset + 3;
			cubeIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> cubeIB = IndexBuffer::Create(cubeIndices, s_Data.MaxIndices);
		s_Data.CubeVertexArray->SetIndexBuffer(cubeIB);
		delete[] cubeIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureShader = Shader::Create("assets/shaders/Renderer3D_Lighting.glsl");

		// Set all texture slots to 0
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.CubeVertexPositions[0] = { -0.5f, -0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPositions[1] = { 0.5f, -0.5f, -0.5f, 1.0f  };
		s_Data.CubeVertexPositions[2] = { 0.5f,  0.5f, -0.5f, 1.0f  };
		s_Data.CubeVertexPositions[3] = { -0.5f,  0.5f, -0.5f, 1.0f };

		s_Data.CubeVertexPositions[4] = { -0.5f, -0.5f, 0.5f, 1.0f  };
		s_Data.CubeVertexPositions[5] = { 0.5f, -0.5f, 0.5f, 1.0f   };
		s_Data.CubeVertexPositions[6] = { 0.5f,  0.5f, 0.5f, 1.0f   };
		s_Data.CubeVertexPositions[7] = { -0.5f, 0.5f, 0.5f, 1.0f   };

		s_Data.CubeVertexPositions[8] = { -0.5f, -0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPositions[9] = { -0.5f, -0.5f, 0.5f, 1.0f  };
		s_Data.CubeVertexPositions[10] = { -0.5f, 0.5f, 0.5f, 1.0f  };
		s_Data.CubeVertexPositions[11] = { -0.5f, 0.5f, -0.5f, 1.0f };

		s_Data.CubeVertexPositions[12] = { 0.5f, -0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPositions[13] = { 0.5f, -0.5f, 0.5f, 1.0f };
		s_Data.CubeVertexPositions[14] = { 0.5f, 0.5f, 0.5f, 1.0f };
		s_Data.CubeVertexPositions[15] = { 0.5f, 0.5f, -0.5f, 1.0f };

		s_Data.CubeVertexPositions[16] = { 0.5f, 0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPositions[17] = { 0.5f, 0.5f, 0.5f, 1.0f };
		s_Data.CubeVertexPositions[18] = { -0.5f, 0.5f, 0.5f, 1.0f };
		s_Data.CubeVertexPositions[19] = { -0.5f, 0.5f, -0.5f, 1.0f };

		s_Data.CubeVertexPositions[20] = { 0.5f, -0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPositions[21] = { 0.5f, -0.5f, 0.5f, 1.0f };
		s_Data.CubeVertexPositions[22] = { -0.5f, -0.5f, 0.5f, 1.0f };
		s_Data.CubeVertexPositions[23] = { -0.5f, -0.5f, -0.5f, 1.0f };

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);
	}

	void Renderer3D::Shutdown()
	{
		PHX_PROFILE_FUNCTION();

		delete[] s_Data.CubeVertexBufferBase;
	}

	void Renderer3D::BeginScene(const OrthographicCamera& camera)
	{
		PHX_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data.TextureShader->SetVec3("u_LightPos", glm::vec3(5.0f));
		s_Data.TextureShader->SetVec3("u_LightColor", glm::vec3(0.8,0.2,0.3));

		StartBatch();
	}

	void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		PHX_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));

		StartBatch();
	}

	void Renderer3D::BeginScene(const EditorCamera& camera)
	{
		PHX_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));

		s_Data.TextureShader->Bind();
		StartBatch(); 
	}

	void Renderer3D::EndScene()
	{
		PHX_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer3D::Flush()
	{
		if (s_Data.CubeIndexCount == 0)
			return; // Nothing to draw

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CubeVertexBufferPtr - (uint8_t*)s_Data.CubeVertexBufferBase);
		s_Data.CubeVertexBuffer->SetData(s_Data.CubeVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		s_Data.TextureShader->Bind();
		RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.CubeIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void phx::Renderer3D::StartBatch()
	{
		s_Data.CubeIndexCount = 0;
		s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer3D::NextBatch()
	{
		
	}

	void Renderer3D::DrawCube(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		PHX_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawCube(transform, color);
	}

	void Renderer3D::DrawCube(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		PHX_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawCube(transform, texture, tilingFactor);
	}

	void Renderer3D::DrawCube(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		PHX_PROFILE_FUNCTION();

		constexpr size_t cubeVertexCount = 24;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (s_Data.CubeIndexCount >= Renderer3DData::MaxIndices)
			NextBatch();

		for (size_t i = 0; i < cubeVertexCount; i++)
		{
			s_Data.CubeVertexBufferPtr->Position = transform * s_Data.CubeVertexPositions[i];
			s_Data.CubeVertexBufferPtr->Color = color;
			s_Data.CubeVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
			s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.CubeVertexBufferPtr->EntityID = entityID;
			s_Data.CubeVertexBufferPtr++;
		}

		s_Data.CubeIndexCount += 36;

		s_Data.Stats.CubeCount++;
	}

	void Renderer3D::DrawRotatedCube(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedCube({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer3D::DrawCube(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		PHX_PROFILE_FUNCTION();

		constexpr size_t cubeVertexCount = 24;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.CubeIndexCount >= Renderer3DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer3DData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < cubeVertexCount; i++)
		{
			s_Data.CubeVertexBufferPtr->Position = transform * s_Data.CubeVertexPositions[i];
			s_Data.CubeVertexBufferPtr->Color = tintColor;
			s_Data.CubeVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
			s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.CubeVertexBufferPtr->EntityID = entityID;
			s_Data.CubeVertexBufferPtr++;
		}

		s_Data.CubeIndexCount += 36;

		s_Data.Stats.CubeCount++;
	}

	void Renderer3D::DrawRotatedCube(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		PHX_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		
		DrawCube(transform, color);
	}

	void Renderer3D::DrawRotatedCube(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedCube({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer3D::DrawRotatedCube(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		PHX_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawCube(transform, texture, tilingFactor, tintColor);
	}

	void Renderer3D::UploadInt(std::string name, int i)
	{
		s_Data.TextureShader->SetInt(name, i);
	}

	void Renderer3D::UploadFloat(std::string name, float f)
	{
		s_Data.TextureShader->SetFloat(name, f);
	}

	void Renderer3D::UploadVec3(std::string name, glm::vec3 v)
	{
		s_Data.TextureShader->SetVec3(name, v);
	}

	void Renderer3D::UploadVec4(std::string name, glm::vec4 v)
	{
		s_Data.TextureShader->SetVec4(name, v);
	}

	void Renderer3D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		if (src.Texture)
		{
			DrawCube(transform, src.Texture, src.TilingFactor, src.Color, entityID);
		}
		else
		{
			DrawCube(transform, src.Color, entityID);
		}
			
	}

	void Renderer3D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer3D::Statistics Renderer3D::GetStats()
	{
		return s_Data.Stats;
	}
}