#include <Phoenix.h>
#include "Phoenix/Application/EntryPoint.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class MainLayer : public phx::Layer
{
public:
	float deltaTimems;
	MainLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{

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
	void OnEvent(phx::Event& e) override
	{
		m_CameraController.OnEvent(e);

		if (e.GetEventType() == phx::EventType::WindowResize)
		{
			auto& re = (phx::WindowResizeEvent&)e;
		
		}
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
		//PushLayer(new MainLayer());
		PushLayer(new Sandbox2D());
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