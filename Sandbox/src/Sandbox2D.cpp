#include "Sandbox2D.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(phx::DeltaTime dt)
{
	m_CameraController.OnUpdate(dt);

	phx::RenderCommand::ClearColor({ 0.1, 0.1, 0.1, 1 });
	phx::RenderCommand::Clear();

	phx::Renderer2D::BeginScene(m_CameraController.GetCamera());
	phx::Renderer2D::DrawQuadFilled({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SquareColor);
	phx::Renderer2D::EndScene();
	//std::dynamic_pointer_cast<phx::OpenGLShader>(m_Shader)->Bind();
	//std::dynamic_pointer_cast<phx::OpenGLShader>(m_Shader)->UploadUniformVec4("u_Color", m_SquareColor);
	//phx::Renderer::Submit(m_Shader, m_SquareVA);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(phx::Event& e)
{
	m_CameraController.OnEvent(e);
}
