#include <filesystem>

#include "SceneHierarchyPanel.h"

#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "Phoenix/Scene/Components.h"
#include "Phoenix/UI/GuiWidgets.h"

#include "Phoenix/Scene/Scene.h"

namespace phx {
	extern const std::filesystem::path s_AssetPath;

	SceneHierarchyPanel::SceneHierarchyPanel()
	{
	}

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);		
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		if (m_Context)
		{
			static ImGuiTextFilter filter;
			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, ImGui::GetFontSize() * 5);
			ImGui::Text("Search:");
			ImGui::NextColumn();
			filter.Draw("##filter", ImGui::GetContentRegionAvail().x);
			ImGui::Columns(1);
			ImGui::Separator();

			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

			if (ImGui::ListBoxHeader("##listbox 1", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 2)))
			{
				UI::DrawItemRowsBackground();
				if (ImGui::IsMouseDown(0) && ImGui::IsItemHovered())
				{
					m_SelectionContext = {};
				}

				m_Context->m_Registry.each([&](auto entityID)
					{
						Entity entity{ entityID , m_Context.get() };
						if (filter.PassFilter(entity.GetComponent<TagComponent>().Tag.c_str()))
						{
							DrawEntityNode(entity);
						}
					});


				if (ImGui::BeginPopupContextWindow(0, 1, false))
				{
					if (ImGui::BeginMenu("Create"))
					{
						if (ImGui::MenuItem("Empty Object"))
						{
							m_Context->CreateEntity("Empty Object");
						}
						if (ImGui::MenuItem("Camera Object"))
						{
							Entity& newEntity = m_Context->CreateEntity("Camera Object");
							newEntity.AddComponent<CameraComponent>();
						}
						switch (m_Context->GetSceneType())
						{
						case Scene::SceneType::Scene2D:
						{
							if (ImGui::BeginMenu("Objects 2D"))
							{
								if (ImGui::MenuItem("Square Object"))
								{
									Entity& newEntity = m_Context->CreateEntity("Square Object");
									newEntity.AddComponent<SpriteRendererComponent>();
								}
								if (ImGui::MenuItem("Circle Object"))
								{
									Entity& newEntity = m_Context->CreateEntity("Circle Object");
									newEntity.AddComponent<CircleRendererComponent>();
								}
								
								ImGui::EndMenu();
							}
							if (ImGui::BeginMenu("Physics 2D"))
							{
								if (ImGui::MenuItem("Static Collider"))
								{
									Entity& newEntity = m_Context->CreateEntity("Static Collider");
									newEntity.AddComponent<SpriteRendererComponent>();
									newEntity.AddComponent<Rigidbody2DComponent>();
									newEntity.GetComponent<Rigidbody2DComponent>().Type = Rigidbody2DComponent::BodyType::Static;
									newEntity.AddComponent<BoxCollider2DComponent>();
								}
								if (ImGui::MenuItem("Square Physics Body"))
								{
									Entity& newEntity = m_Context->CreateEntity("Square Physics Body");
									newEntity.AddComponent<SpriteRendererComponent>();
									newEntity.AddComponent<Rigidbody2DComponent>();
									newEntity.GetComponent<Rigidbody2DComponent>().Type = Rigidbody2DComponent::BodyType::Dynamic;
									newEntity.AddComponent<BoxCollider2DComponent>();
								}
								if (ImGui::MenuItem("Circle Physics Body"))
								{
									Entity& newEntity = m_Context->CreateEntity("Circle Physics Body");
									newEntity.AddComponent<CircleRendererComponent>();
									newEntity.AddComponent<Rigidbody2DComponent>();
									newEntity.GetComponent<Rigidbody2DComponent>().Type = Rigidbody2DComponent::BodyType::Dynamic;
									newEntity.AddComponent<CircleCollider2DComponent>();
								}
								ImGui::EndMenu();
							}
							break;
						}
						}
						ImGui::EndMenu();
					}
					ImGui::EndPopup();
				}
				ImGui::ListBoxFooter();
			}
			ImGui::PopStyleColor();

			int sizeX = 200;
			int sizeY = 300;

			ImGui::SameLine(0.1f);
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - sizeX, ImGui::GetWindowSize().y + ImGui::GetScrollY() - sizeY));

			//ImGui::Image((void*)m_Alstolfo->GetRendererID(), ImVec2(200, 300), ImVec2{ 1, 1 }, ImVec2{ 0, 0 }, ImVec4(1.0f, 1.0f, 1.0f, 0.4f));
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			ImGui::Text(entity.GetComponent<TagComponent>().Tag.c_str());
			ImGui::Separator();

			if (ImGui::MenuItem("Duplicate"))
				m_Context->DuplicateEntity(entity);

			if (ImGui::MenuItem("Delete"))
				entityDeleted = true;
			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}			
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction, bool canDelete = true)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed;
		auto& component = entity.GetComponent<T>();
		ImVec2 cr = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

		ImGui::Separator();

		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		if (ImGui::IsItemClicked(1))
		{
			ImGui::OpenPopup("ComponentSettings");
		}
		ImGui::PopStyleVar();

		ImGui::SameLine(cr.x - lineHeight * 0.5f);
		if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (canDelete)
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;
			}
			else
			{
				ImGui::MenuItem("Can't Remove", 0, false, false);
			}

			ImGui::EndPopup();
		}
		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent)
			entity.RemoveComponent<T>();
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (!m_SelectionContext.HasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			
			switch (m_Context->GetSceneType())
			{
			case Scene::SceneType::Scene2D:
			{
				if (ImGui::BeginMenu("Rendering"))
				{
					if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
					{
						if (ImGui::MenuItem("Sprite Renderer"))
						{
							m_SelectionContext.AddComponent<SpriteRendererComponent>();
							ImGui::CloseCurrentPopup();
						}
					}
					if (!m_SelectionContext.HasComponent<CircleRendererComponent>())
					{
						if (ImGui::MenuItem("Circle Renderer"))
						{
							m_SelectionContext.AddComponent<CircleRendererComponent>();
							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Physics"))
				{
					if (!m_SelectionContext.HasComponent<Rigidbody2DComponent>())
					{
						if (ImGui::MenuItem("Rigidbody 2D"))
						{
							m_SelectionContext.AddComponent<Rigidbody2DComponent>();
							ImGui::CloseCurrentPopup();
						}
					}

					if (!m_SelectionContext.HasComponent<BoxCollider2DComponent>())
					{
						if (ImGui::MenuItem("Box Collider 2D"))
						{
							m_SelectionContext.AddComponent<BoxCollider2DComponent>();
							if (!m_SelectionContext.HasComponent<Rigidbody2DComponent>())
								m_SelectionContext.AddComponent<Rigidbody2DComponent>();
							ImGui::CloseCurrentPopup();
						}
					}
					if (!m_SelectionContext.HasComponent<CircleCollider2DComponent>())
					{
						if (ImGui::MenuItem("Circle Collider 2D"))
						{
							m_SelectionContext.AddComponent<CircleCollider2DComponent>();
							if (!m_SelectionContext.HasComponent<Rigidbody2DComponent>())
								m_SelectionContext.AddComponent<Rigidbody2DComponent>();
							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::EndMenu();
				}
				break;
			}
			}

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();


		if (entity.HasComponent<TransformComponent>())
		{
			DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
				{
					UI::DrawVec3Controls("Translation", component.Translation);
					glm::vec3 rotation = glm::degrees(component.Rotation);
					UI::DrawVec3Controls("Rotation", rotation);
					component.Rotation = glm::radians(rotation);
					UI::DrawVec3Controls("Scale", component.Scale, 1.0f);
				}, false);
		}
		if (entity.HasComponent<CameraComponent>())
		{
			DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
				{
					auto& camera = component.Camera;

					const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
					const char* currentProjectionTypeString = projectionTypeStrings[(int)component.Camera.GetProjectionType()];

					UI::DrawCheckbox("Active", &component.Primary);

					ImGui::PushID("Projection");
					ImGui::Columns(2);
					ImGui::SetColumnWidth(0, 100.0f);
					ImGui::Text("Projection");

					ImGui::NextColumn();
					ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
					if (ImGui::BeginCombo("##Projection", currentProjectionTypeString))
					{
						for (int i = 0; i < 2; i++)
						{
							bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
							if (ImGui::Selectable(projectionTypeStrings[i], &isSelected))
							{
								currentProjectionTypeString = projectionTypeStrings[i];
								camera.SetProjectionType((SceneCamera::ProjectionType)i);
							}
							if (isSelected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
					ImGui::PopItemWidth();
					ImGui::PopID();
					ImGui::Columns(1);

					if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
					{
						float perspectiveFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
						if (UI::DrawDragFloat("Vertical FOV", &perspectiveFOV))
							camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveFOV));

						float perspectiveNear = camera.GetPerspectiveNearClip();
						if (UI::DrawDragFloat("Near", &perspectiveNear))
							camera.SetPerspectiveNearClip(perspectiveNear);

						float perspectiveFar = camera.GetPerspectiveFarClip();
						if (UI::DrawDragFloat("Far", &perspectiveFar))
							camera.SetPerspectiveFarClip(perspectiveFar);
					}
					if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
					{
						float orthoSize = camera.GetOrthographicSize();
						if (UI::DrawDragFloat("Size", &orthoSize))
							camera.SetOrthographicSize(orthoSize);

						float orthoNear = camera.GetOrthographicNearClip();
						if (UI::DrawDragFloat("Near", &orthoNear))
							camera.SetOrthographicNearClip(orthoNear);

						float orthoFar = camera.GetOrthographicFarClip();
						if (UI::DrawDragFloat("Far", &orthoFar))
							camera.SetOrthographicFarClip(orthoFar);

						UI::DrawCheckbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
					}
				});
		}
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
			{
					UI::DrawColorControls("Color", component.Color);

					ImGui::Columns(2);
					ImGui::SetColumnWidth(0, 100.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));
					ImGui::Text("Texture");
					ImGui::NextColumn();		
					if(!component.Path.empty())
						ImGui::Button(component.Path.c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 25, 25));
					else
						ImGui::Button("Drag Image", ImVec2(ImGui::GetContentRegionAvail().x - 25, 25));

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
						{
							const wchar_t* path = (const wchar_t*)payload->Data;
							std::filesystem::path texturePath = std::filesystem::path(s_AssetPath) / path;
							Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
							if (texture->IsLoaded())
							{
								component.Texture = texture;
								component.Path = texturePath.string();
							}
							else
								PHX_CORE_WARN("Could not load texture {0}", texturePath.filename().string());
						}

						ImGui::EndDragDropTarget();
					}
					ImGui::Columns(1);

					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
					ImGui::SameLine();

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.25f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.3f, 0.3f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.2f, 0.25f, 1.0f });
					ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
					if (ImGui::Button("X", ImVec2(25, 25)))
					{
						component.Texture = nullptr;
						component.Path = std::string();
					}
					ImGui::PopFont();
					ImGui::PopStyleColor(3);
					ImGui::PopStyleVar(2);
					
					UI::DrawGap();

					UI::DrawDragFloat("Tiling Factor", &component.TilingFactor, 0.1f);
			});
		}
		if (entity.HasComponent<CircleRendererComponent>())
		{
			DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](auto& component)
				{
					UI::DrawVec4ColorControls("Color", component.Color);
					UI::DrawGap();
					UI::DrawDragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
					float fade = component.Fade * 1000;
					UI::DrawDragFloat("Fade", &fade, 2.0f, 0.0f, 100.0f);
					component.Fade = fade / 1000;
				});
		}
		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component)
				{
					const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
					const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
					ImGui::Columns(2);
					ImGui::SetColumnWidth(0, 100.0f);
					ImGui::Text("Body Type");

					ImGui::NextColumn();
					ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
					if (ImGui::BeginCombo("##Body Type", currentBodyTypeString))
					{
						for (int i = 0; i < 3; i++)
						{
							bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
							if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
							{
								currentBodyTypeString = bodyTypeStrings[i];
								component.Type = (Rigidbody2DComponent::BodyType)i;
							}

							if (isSelected)
								ImGui::SetItemDefaultFocus();
						}

						ImGui::EndCombo();
					}  
					ImGui::PopItemWidth();
					ImGui::Columns(1);
					UI::DrawCheckbox("Fixed Rotation", &component.FixedRotation);
				});
		}
		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
				{
					if (ImGui::TreeNodeEx("Physics Settings", ImGuiTreeNodeFlags_SpanAvailWidth))
					{
						UI::DrawDragFloat("Density", &component.Density, 0.01f, 0.0f, 100.0f);
						UI::DrawDragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
						UI::DrawDragFloat("Bounce", &component.Restitution, 0.01f, 0.0f, 1.0f);
						UI::DrawDragFloat("Bounce Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
						ImGui::TreePop();
					}
					UI::DrawVec2Controls("Size", component.Size, 0.5f);
					UI::DrawVec2Controls("Offset", component.Offset);
					UI::DrawGap();
					UI::DrawCheckbox("Is Sensor", &component.IsSensor);
					UI::DrawCheckbox("Show Collider", &component.ShowCollider);
				});
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& component)
				{
					if (ImGui::TreeNodeEx("Physics Settings", ImGuiTreeNodeFlags_SpanAvailWidth))
					{
						UI::DrawDragFloat("Density", &component.Density, 0.01f, 0.0f, 100.0f);
						UI::DrawDragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
						UI::DrawDragFloat("Bounce", &component.Restitution, 0.01f, 0.0f, 1.0f);
						UI::DrawDragFloat("Bounce Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
						ImGui::TreePop();
					}
					UI::DrawDragFloat("Radius", &component.Radius);
					UI::DrawVec2Controls("Offset", component.Offset);
					UI::DrawGap();
					UI::DrawCheckbox("Is Sensor", &component.IsSensor);
					UI::DrawCheckbox("Show Collider", &component.ShowCollider);
				});
		}
	}
}