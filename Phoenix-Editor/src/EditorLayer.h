#pragma once

#include <Phoenix.h>

namespace phx {
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate(phx::DeltaTime dt) override;
		virtual void OnImGuiRender() override;
		void OnEvent(phx::Event& e) override;
	private:
		OrthographicCameraController m_CameraController;
		DiscordRPC m_RPC;

		Ref<Shader> m_Shader;
		Ref<VertexArray> m_SquareVA;
		Ref<Framebuffer> m_Framebuffer;	

		float tiling = 5.0f;
		float deltatimems;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		Ref<Texture2D> m_Texture;

	};
}
