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
		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
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

		m_TextureShader = phx::Shader::Create("assets/shaders/texture.glsl");
		m_Texture = phx::Texture2D::Create("assets/textures/doggo.png");

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

		m_CameraController.OnUpdate(dt);

		phx::RenderCommand::ClearColor({ 0.1, 0.1, 0.1, 1 });
		phx::RenderCommand::Clear();

		phx::Renderer::BeginScene(m_CameraController.GetCamera());

		m_Texture->Bind();
		phx::Renderer::Submit(m_TextureShader, m_SquareVA);

		phx::Renderer::EndScene();
	}
private:
	phx::Ref<phx::Shader> m_TextureShader;
	phx::Ref<phx::VertexArray> m_SquareVA;

	phx::Ref<phx::Texture2D> m_Texture;

	phx::OrthographicCameraController m_CameraController;

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