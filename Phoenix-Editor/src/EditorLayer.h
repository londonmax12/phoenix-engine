#pragma once
#include <Phoenix.h>
#include <Phoenix/Renderer/EditorCamera.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

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
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void SaveSceneAs();
		void SaveScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);

		OrthographicCameraController m_CameraController;

		Ref<DiscordRPC> m_RPC;

		Ref<Shader> m_Shader;
		Ref<VertexArray> m_SquareVA;
		Ref<Framebuffer> m_Framebuffer;	

		Ref<Scene> m_ActiveScene;

		Entity m_HoveredEntity;

		bool m_PrimaryCamera = true;

		EditorCamera m_EditorCamera;

		float tiling = 5.0f;
		float deltatimems;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		bool m_ShowMetrics = true;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		int m_GizmoType = -1;

		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;
	};
}
