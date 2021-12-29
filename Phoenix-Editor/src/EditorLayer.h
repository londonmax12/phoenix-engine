#pragma once
#include <Phoenix.h>
#include <Phoenix/Renderer/EditorCamera.h>
#include <Phoenix/Audio/SoundSource.h>
#include <Phoenix/Time/Timer.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ShaderEditorPanel.h"
#include "Panels/ConsolePanel.h"

#include <string>

#include "Phoenix/Renderer/Mesh.h"

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

		void OnOverlayRender();

		void NewScene(Scene::SceneType type, bool AddCamera = false);
		void SaveSceneAs();
		void SaveScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

		void OnScenePlay();
		void OnSceneStop();
		void OnScenePlayTest();

		void OnDuplicateEntity();

		void UI_Toolbar();

		OrthographicCameraController m_CameraController;

		Ref<Shader> m_Shader;
		Ref<VertexArray> m_SquareVA;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;

		Ref<Project> m_Project;

		std::filesystem::path m_EditorScenePath;

		Entity m_HoveredEntity;

		bool m_PrimaryCamera = true;

		EditorCamera m_EditorCamera;

		float tiling = 5.0f;
		float deltatimems;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		bool m_ShowMetrics = true;
		bool m_ShowContentBrowser = true;
		bool m_ShowShaderEditor = false;
		bool m_ShowConsole = false;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		int m_GizmoType = -1;

		bool m_ShowPhysicsColliders = true;

		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;
		ShaderEditorPanel m_ShaderEditorPanel;
		ConsolePanel m_ConsolePanel;

		float m_GravityX = 0.0f;
		float m_GravityY = -9.8f;
		enum class SceneState
		{
			Edit = 0, Play = 1, PhysicTest = 2
		};

		SceneState m_SceneState = SceneState::Edit;

		// Resources
		Ref<Texture2D> m_PlayIcon, m_StopIcon, m_PlayTestIcon;
	};
}
