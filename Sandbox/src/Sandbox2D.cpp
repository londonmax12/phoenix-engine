#include "Sandbox2D.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	PHX_PROFILE_FUNCTION();

	m_Texture = phx::Texture2D::Create("assets/textures/rock.png");
}

void Sandbox2D::OnDetach()
{
	PHX_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(phx::DeltaTime dt)
{
	PHX_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(dt);

	{
		PHX_PROFILE_SCOPE("Renderer Prep");
		phx::RenderCommand::ClearColor({ 0.1, 0.1, 0.1, 1 });
		phx::RenderCommand::Clear();
	}

	{
		PHX_PROFILE_SCOPE("Renderer Draw");

		static float rotation = 0.0f;
		rotation += 30 * dt;

		phx::Renderer2D::BeginScene(m_CameraController.GetCamera());
		phx::Renderer2D::DrawQuadFilled({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		phx::Renderer2D::DrawQuadFilled({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.8f, 0.3f, 1.0f });
		phx::Renderer2D::DrawRotatedQuadFilled({ 0.0f, 0.0f, -0.01f }, { 10.0f, 10.0f }, rotation, m_Texture, tiling, m_SquareColor);
		phx::Renderer2D::DrawRotatedQuadFilled({ 0.0f, 0.0f, -0.005f }, { 5.0f, 5.0f }, rotation, m_SquareColor);

		phx::Renderer2D::EndScene();
	}
	
}

void Sandbox2D::OnImGuiRender()
{
	PHX_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Tint Color", glm::value_ptr(m_SquareColor));
	ImGui::SliderFloat("Tiling", &tiling, 0.2f, 10.0f);
	ImGui::End();
}

void Sandbox2D::OnEvent(phx::Event& e)
{
	m_CameraController.OnEvent(e);
}
