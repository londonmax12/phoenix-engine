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

		Ref<DiscordRPC> m_RPC;

		Ref<Shader> m_Shader;
		Ref<VertexArray> m_SquareVA;
		Ref<Framebuffer> m_Framebuffer;	

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;

		float tiling = 5.0f;
		float deltatimems;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	};
}
