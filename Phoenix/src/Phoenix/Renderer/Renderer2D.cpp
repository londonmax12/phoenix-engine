#include "phxpch.h"
#include "Renderer2D.h"

#include "Phoenix/Renderer/VertexArray.h"
#include "Phoenix/Renderer/Shader.h"
#include "Phoenix/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace phx 
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatShader;

	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();
		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ phx::ShaderDataType::vec3, "a_Position" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->FlatShader = Shader::Create("assets/shaders/shader.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<phx::OpenGLShader>(s_Data->FlatShader)->Bind();
		std::dynamic_pointer_cast<phx::OpenGLShader>(s_Data->FlatShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<phx::OpenGLShader>(s_Data->FlatShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuadFilled(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuadFilled({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer2D::DrawQuadFilled(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<phx::OpenGLShader>(s_Data->FlatShader)->Bind();
		std::dynamic_pointer_cast<phx::OpenGLShader>(s_Data->FlatShader)->UploadUniformVec4("u_Color", color);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}

