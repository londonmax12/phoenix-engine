#pragma once

#include <Phoenix.h>

class Sandbox2D : public phx::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(phx::DeltaTime dt) override;
	virtual void OnImGuiRender() override;
	void OnEvent(phx::Event& e) override;
private:	
	phx::OrthographicCameraController m_CameraController;

	phx::Ref<phx::Shader> m_Shader;
	phx::Ref<phx::VertexArray> m_SquareVA;	

	float tiling = 1.0f;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	phx::Ref<phx::Texture2D> m_Texture;

};