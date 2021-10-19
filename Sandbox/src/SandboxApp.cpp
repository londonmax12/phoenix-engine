#include <Phoenix.h>

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

class MainLayer : public phx::Layer
{
public:
	float deltaTimems;
	MainLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_SquareVA.reset(phx::VertexArray::Create());
	
		float squareVertices[5 * 4] = {
			-0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
			 0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
			 0.75f,  0.75f, 0.0f, 1.0f, 1.0f,
			-0.75f,  0.75f, 0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<phx::VertexBuffer> squareVB;
		squareVB.reset(phx::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ phx::ShaderDataType::vec3, "a_Position" },
			{ phx::ShaderDataType::vec2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<phx::IndexBuffer> squareIB;
		squareIB.reset(phx::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
	
			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
						#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			
			uniform vec3 u_Color;
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";
		m_BlueShader.reset(phx::Shader::Create(blueShaderVertexSrc, blueShaderFragmentSrc));

		std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec2 v_TexCoord;
			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec2 v_TexCoord;
			
			uniform sampler2D u_Texture;
			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(phx::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

		m_Texture = phx::Texture2D::Create("assets/textures/doggo.png");
		m_Texture2 = phx::Texture2D::Create("assets/textures/r6sm.png");

		std::dynamic_pointer_cast<phx::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<phx::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}
	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Metrics");
		ImGui::Text(("FPS: " + std::to_string(1000.0f / deltaTimems)).c_str());
		ImGui::End();
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
	void OnEvent(phx::Event& event) override
	{
	}

	void OnUpdate(phx::DeltaTime dt) override
	{
		deltaTimems = dt.GetMilliseconds();
		if (phx::Input::IsKeyPressed(PHX_KEY_A))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * dt;
		}
		if (phx::Input::IsKeyPressed(PHX_KEY_D))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * dt;
		}
		if (phx::Input::IsKeyPressed(PHX_KEY_W))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * dt;
		}
		if (phx::Input::IsKeyPressed(PHX_KEY_S))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * dt;
		}
		if (phx::Input::IsKeyPressed(PHX_KEY_Q))
		{
			m_CameraRotation += m_CameraRotateSpeed * dt;
		}
		if (phx::Input::IsKeyPressed(PHX_KEY_E))
		{
			m_CameraRotation -= m_CameraRotateSpeed * dt;
		}

		phx::RenderCommand::ClearColor({ 0.1, 0.1, 0.1, 1 });
		phx::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		phx::Renderer::BeginScene(m_Camera);

		std::dynamic_pointer_cast<phx::OpenGLShader>(m_BlueShader)->Bind();
		std::dynamic_pointer_cast<phx::OpenGLShader>(m_BlueShader)->UploadUniformVec3("u_Color", m_SquareColor);

		m_Texture->Bind();
		phx::Renderer::Submit(m_TextureShader, m_SquareVA);
		m_Texture2->Bind();
		phx::Renderer::Submit(m_TextureShader, m_SquareVA);
		phx::Renderer::EndScene();
	}
private:
	phx::Ref<phx::Shader> m_BlueShader, m_TextureShader;
	phx::Ref<phx::VertexArray> m_SquareVA;

	phx::Ref<phx::Texture2D> m_Texture, m_Texture2;

	phx::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;

	float m_CameraRotateSpeed = 40.0f;
	float m_CameraMoveSpeed = 2.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public phx::Application
{
public:
	Sandbox()
	{
		PushLayer(new MainLayer());
	}
	~Sandbox()
	{

	}
	void OnInit() override {
		
	}
};

phx::Application* phx::CreateApplication()
{
	return new Sandbox();
}