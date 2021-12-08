#include "EditorLayer.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Phoenix/Math/Math.h"

#include <chrono>

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

#include "Panels/ThemeEditor.h"

float highfps = 0;

namespace phx
{
	extern const std::filesystem::path s_AssetPath;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		PHX_PROFILE_FUNCTION();
		
		m_PlayIcon = Texture2D::Create("resources/icons/editor-layer/play-icon.png");
		m_StopIcon = Texture2D::Create("resources/icons/editor-layer/stop-icon.png");
		m_PlayTestIcon = Texture2D::Create("resources/icons/editor-layer/playtest-icon.png");
		
		FramebufferSpecification framebufferSpec;
		framebufferSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		framebufferSpec.Width = 1280;
		framebufferSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(framebufferSpec);

		m_ActiveScene = CreateRef<Scene>();

		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(sceneFilePath);
		}

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		NewScene(Scene::SceneType::Scene2D, true);

		/*Entity entity = m_ActiveScene->CreateEntity("Red Square");
		entity.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		class Controller : public ScriptableEntity
		{
		public:
			virtual void OnCreate() override
			{
			}

			virtual void OnDestroy() override
			{
			}

			virtual void OnUpdate(DeltaTime dt) override
			{
				auto& rb2d = GetComponent<Rigidbody2DComponent>();
				float speed = 5.0f;

				if (Input::IsKeyPressed(Key::A))
					if (rb2d.Force.x > -5)
						rb2d.ApplyForce({ -(speed * dt), 0 });
				if (Input::IsKeyPressed(Key::D))
					if (rb2d.Force.x < 5)
						rb2d.ApplyForce({ (speed * dt), 0 });				
				if (Input::IsKeyPressed(Key::W))
						rb2d.ApplyForce({ 0, (speed * dt) });
				if (Input::IsKeyPressed(Key::S))
					rb2d.ApplyForce({ 0, -(speed * dt) });
			}
		};
		entity.AddComponent<NativeScriptComponent>().Bind<Controller>();*/

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		PHX_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(DeltaTime dt)
	{
		PHX_PROFILE_FUNCTION();

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		deltatimems = dt.GetMilliseconds();


		Renderer2D::ResetStats();
		Renderer3D::ResetStats();

		m_Framebuffer->Bind();
		RenderCommand::ClearColor({ 0.12, 0.12, 0.12, 1 });
		RenderCommand::Clear();

		m_Framebuffer->ClearAttachment(1, -1);
		//Renderer2D::BeginScene(m_CameraController.GetCamera());

		switch (m_SceneState)
		{
		case phx::EditorLayer::SceneState::Edit:
		{

			m_EditorCamera.OnUpdate(dt);

			m_ActiveScene->OnUpdateEditor(dt, m_EditorCamera);
			break;
		}			
		case phx::EditorLayer::SceneState::Play:
		{
			m_ActiveScene->OnUpdateRuntime(dt);
			break;
		}
		case phx::EditorLayer::SceneState::PhysicTest:
		{
			m_EditorCamera.OnUpdate(dt);

			m_ActiveScene->OnUpdatePhysics(dt, m_EditorCamera);
			break;
		}
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

		//Renderer2D::EndScene();
		OnOverlayRender();

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		PHX_PROFILE_FUNCTION();

		//ImGui::ShowDemoWindow();

		static bool p_open = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::BeginMenu("New"))
				{
					if (ImGui::MenuItem("2D Scene"))
					{
						NewScene(Scene::SceneType::Scene2D, true);
					}
					if (ImGui::MenuItem("3D Scene", "Ctrl+N"))
					{
						NewScene(Scene::SceneType::Scene3D);
					}	
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					SaveScene();
				}
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Quit", "Alt+F4"))
				{ 
					Application::Get().Close(); 
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::BeginMenu("Windows"))
				{
					ImGui::MenuItem("Console", NULL, &m_ShowConsole);
					ImGui::MenuItem("Content Browser", NULL, &m_ShowContentBrowser);
					ImGui::MenuItem("Shader Editor", NULL, &m_ShowShaderEditor);
					ImGui::Separator();
					ImGui::MenuItem("Metrics", NULL, &m_ShowMetrics);
					ImGui::EndMenu();
				}		
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


		/*ImGui::Begin("test");
		if (ImGui::Button("Play"))
		{
			SoundDevice* mysounddevice = SoundDevice::New();

			uint32_t sound1 = SoundBuffer::New()->AddSoundEffect("C:/Users/London/Desktop/Phoenixx/Phoenix-Editor/assets/audio/laser1.wav");

			SoundSource mySpeaker;

			mySpeaker.Play(sound1);
		}
		ImGui::End();*/

		m_SceneHierarchyPanel.OnImGuiRender();

		if (m_ShowContentBrowser)
			m_ContentBrowserPanel.OnImGuiRender();

		if (m_ShowShaderEditor)
			m_ShaderEditorPanel.OnImGuiRender();

		if (m_ShowConsole)
			m_ConsolePanel.OnImGuiRender();

		//ImGui::ShowDemoWindow();

		ImGui::Begin("Settings");
		ImGui::Checkbox("Show physics colliders", &m_ShowPhysicsColliders);
		ImGui::End();

		if (m_ShowMetrics)
		{
			ImGui::Begin("Metrics");
			float fps = 1000.0f / deltatimems;

			if (fps > highfps)
				highfps = fps;

			auto stats = Renderer2D::GetStats();
			auto stats3d = Renderer3D::GetStats();

			ImGui::Text("FPS: %.0f", fps);
			ImGui::SameLine();
			ImGui::Text("High: %.0f", highfps);
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
				highfps = 0;
			ImGui::Separator();
			ImGui::Text("Renderer2D Stats");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			ImGui::Separator();

			ImGui::Text("Renderer3D Stats");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Cubes: %d", stats.QuadCount);
			ImGui::Separator();

			ImGui::Text("Scene Stats");
			ImGui::Text("Registry Size: %d", m_ActiveScene->GetRegistrySize());
			std::string name = "None";
			if (m_HoveredEntity)
				name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
			ImGui::Text("Hovered Entity: %s", name.c_str());
			ImGui::End();
		}

		//renderThemeEditor();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

		ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiDockNodeFlags_HiddenTabBar | ImGuiDockNodeFlags_CentralNode | ImGuiDockNodeFlags_AutoHideTabBar | ImGuiWindowFlags_NoDecoration);

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockMouseEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
		
				std::filesystem::path fsPath = std::filesystem::path(s_AssetPath) / path;
				std::string ext = fsPath.extension().string();

				if (ext == ".phoenix")
				{
					if (m_SceneState == SceneState::Play)
					{
						m_SceneState = SceneState::Edit;
					}
					OpenScene(fsPath);
				}
				else if (ext == ".png" || ext == ".jpg" || ext == ".bmp")
				{
					if(m_HoveredEntity)
						if (m_HoveredEntity.HasComponent<SpriteRendererComponent>())
						{
							auto& src = m_HoveredEntity.GetComponent<SpriteRendererComponent>();
							src.Texture = Texture2D::Create(fsPath.string());
							src.Path = fsPath.string();
						}
				}
			}		

			ImGui::EndDragDropTarget();
		}

		// Gizmos
		if (m_SceneState == SceneState::Edit)
		{
			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity && m_GizmoType != -1)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

				const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
				glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

				// Entity transform
				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetTransform();

				// Snapping
				bool snap = Input::IsKeyPressed(Key::LeftControl);
				float snapValue = 0.5f; // Snap to 0.5m for translation/scale
				// Snap to 15 degrees for rotation
				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 15.0f;

				float snapValues[3] = { snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
					nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 translation, rotation, scale;
					Math::DecomposeTransform(transform, translation, rotation, scale);

					glm::vec3 deltaRotation = rotation - tc.Rotation;
					tc.Translation = translation;
					tc.Rotation += deltaRotation;
					tc.Scale = scale;
				}
			}
		}
		
		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(PHX_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(PHX_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}
	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));


		ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		Ref<Texture2D> icon = m_SceneState == SceneState::Edit ? m_PlayIcon : m_StopIcon;

		float xPos1 = (ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5);
		float xPos2 = (ImGui::GetContentRegionMax().x * 0.5f) + (size * 0.5);

		ImGui::SetCursorPosX(xPos1);
		if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (m_SceneState == SceneState::Edit)
			{
				OnScenePlay();
			}
			else if (m_SceneState == SceneState::Play || m_SceneState == SceneState::PhysicTest)
			{
				OnSceneStop();
			}
		}
		if (m_SceneState == SceneState::Edit)
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Play");
				ImGui::EndTooltip();
			}
		
		ImGui::SameLine();
		ImGui::SetCursorPosX(xPos2);
		if (ImGui::ImageButton((ImTextureID)m_PlayTestIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0))
		{
			if (m_SceneState == SceneState::Edit)
			{
				OnScenePlayTest();
			}
			else if (m_SceneState == SceneState::Play || m_SceneState == SceneState::PhysicTest)
			{
				OnSceneStop();
			}
		}
		if (m_SceneState == SceneState::Edit)
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Playtest");
				ImGui::EndTooltip();
			}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		
		ImGui::End();
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		//	ShortCuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
		case (int)Key::S:
		{
			if (control && shift)
			{
				SaveSceneAs();
			}
			else if (control)
			{
				SaveScene();
			}
			break;
		}

		case (int)Key::O:
		{
			if (control)
			{
				OpenScene();
			}
			break;
		}
		case (int)Key::N:
		{
			if (control)
			{
				NewScene(Scene::SceneType::Scene3D);
			}
			break;
		}
		case (int)Key::GraveAccent:
		{
			Entity entity = m_SceneHierarchyPanel.GetSelectedEntity();
			if (entity)
			{
				m_EditorCamera.SetPosition(entity.GetComponent<TransformComponent>().Translation);
				m_EditorCamera.SetDistance(10.0f);
			}
			break;
		}

		case (int)Key::D1:
		{
			if (!ImGuizmo::IsUsing())
			{
				m_GizmoType = -1;				
			}
			break;
		}
			
		case (int)Key::D2:
		{
			if (!ImGuizmo::IsUsing())
			{
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;				
			}
			break;
		}
			
		case (int)Key::D3:
		{
			if (!ImGuizmo::IsUsing())
			{
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;			
			}
			break;
		}
			
		case (int)Key::D4:
		{
			if (!ImGuizmo::IsUsing())
			{
				m_GizmoType = ImGuizmo::OPERATION::SCALE;	
			}
			break;
		}
		case (int)Key::R:
		{
			m_EditorCamera.Reset();
			break;
		}
		case (int)Key::Delete:
		{
			if (m_SceneHierarchyPanel.GetSelectedEntity())
			{
				m_ActiveScene->DestroyEntity(m_SceneHierarchyPanel.GetSelectedEntity());
				m_SceneHierarchyPanel.SetSelectedEntity();
			}
			break;
		}
		case (int)Key::D:
		{
			if (control)
				OnDuplicateEntity();
			break;
		}
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == (int)Mouse::ButtonLeft)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}

	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		if (m_ShowPhysicsColliders)
		{
			// Box Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					if (bc2d.ShowCollider)
					{
						glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.001f);
						glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

						glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
							* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
							* glm::scale(glm::mat4(1.0f), scale);

						Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
					}
				}
			}

			// Circle Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

					if (cc2d.ShowCollider)
					{
						glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.001f);
						glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);

						glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
							* glm::scale(glm::mat4(1.0f), scale);

						Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.03f / (cc2d.Radius / 2));
					}	
				}
			}
		}

		Renderer2D::EndScene();
	}

	void EditorLayer::NewScene(Scene::SceneType type, bool AddCamera)
	{
		switch (type)
		{
		case phx::Scene::SceneType::Scene2D:
		{
			Renderer2D::Init();
			break;
		}
		case phx::Scene::SceneType::Scene3D:
		{
			Renderer3D::Init();
			break;
		}
		}
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_ActiveScene->SetSceneType(type);
		if (AddCamera)
		{
			Entity cam = m_ActiveScene->CreateEntity("Main Camera");
			cam.AddComponent<CameraComponent>();
		}
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		m_EditorScenePath = std::filesystem::path();
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Phoenix Scene (*.phoenix)\0*.phoenix\0");
		if (!filepath.empty())
		{
			SerializeScene(m_ActiveScene, filepath);
			m_EditorScenePath = filepath;
		}
	}

	void EditorLayer::SaveScene()
	{
		if (!m_EditorScenePath.empty())
			SerializeScene(m_ActiveScene, m_EditorScenePath);
		else
			SaveSceneAs();
	}


	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneSerializer serializer(scene);
		serializer.Serialize(path.string());
	}

	void EditorLayer::OpenScene()
	{

		std::string filepath = FileDialogs::OpenFile("Phoenix Scene (*.phoenix)\0*.phoenix\0");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		if (path.extension().string() != ".phoenix")
		{
			PHX_CORE_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.Deserialize(path.string()))
		{
			switch (newScene->GetSceneType())
			{
			case phx::Scene::SceneType::Scene2D:
			{
				Renderer2D::Init();
				break;
			}
			case phx::Scene::SceneType::Scene3D:
			{
				Renderer3D::Init();
				break;
			}
			}
			m_EditorScene = newScene;
			m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_EditorScene);

			m_ActiveScene = m_EditorScene;
			m_EditorScenePath = path;
		}
	}

	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;

		m_EditorScene = m_ActiveScene;
		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}
	void EditorLayer::OnScenePlayTest()
	{
		m_SceneState = SceneState::PhysicTest;

		m_EditorScene = m_ActiveScene;
		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}
	void EditorLayer::OnSceneStop()
	{
		m_SceneState = SceneState::Edit;

		m_ActiveScene->OnRuntimeStop();
		m_ActiveScene = m_EditorScene;

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
			m_EditorScene->DuplicateEntity(selectedEntity);
	}
}