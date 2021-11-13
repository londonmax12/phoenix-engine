#pragma once
#include <Phoenix.h>
#include <Phoenix/Renderer/EditorCamera.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ShaderEditorPanel.h"

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

		void NewScene(Scene::SceneType type);
		void SaveSceneAs();
		void SaveScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

		void OnScenePlay();
		void OnSceneStop();

		void OnDuplicateEntity();

		void UI_Toolbar();

		OrthographicCameraController m_CameraController;

		Ref<DiscordRPC> m_RPC;

		Ref<Shader> m_Shader;
		Ref<VertexArray> m_SquareVA;
		Ref<Framebuffer> m_Framebuffer;	

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;

		std::filesystem::path m_EditorScenePath;

		Entity m_HoveredEntity;

		bool m_PrimaryCamera = true;

		EditorCamera m_EditorCamera;

		float tiling = 5.0f;
		float deltatimems;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		bool m_ShowShaderEditor = true;
		bool m_ShowMetrics = true;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		int m_GizmoType = -1;

		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;
		ShaderEditorPanel m_ShaderEditorPanel;

		enum class SceneState
		{
			Edit = 0, Play = 1
		};

		SceneState m_SceneState = SceneState::Edit;

		// Resources
		Ref<Texture2D> m_PlayIcon, m_StopIcon;
	};
}
