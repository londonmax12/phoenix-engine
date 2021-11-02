#pragma once

#include <Phoenix.h>
#include "Panels/SceneHierarchyPanel.h"

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
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void SaveSceneAs();
		void SaveScene();
		void OpenScene();

		OrthographicCameraController m_CameraController;

		Ref<DiscordRPC> m_RPC;

		Ref<Shader> m_Shader;
		Ref<VertexArray> m_SquareVA;
		Ref<Framebuffer> m_Framebuffer;	

		Ref<Scene> m_ActiveScene;

		Entity m_CameraEntity;
		Entity m_SecondCamera;

		bool m_PrimaryCamera = true;

		float tiling = 5.0f;
		float deltatimems;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		int m_GizmoType = -1;

		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}
