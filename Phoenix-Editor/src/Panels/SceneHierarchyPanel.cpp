#include <filesystem>

#include "SceneHierarchyPanel.h"

#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "Phoenix/Scene/Components.h"
#include "Phoenix/ImGui/GuiWidgets.h"

namespace phx {

	extern const std::filesystem::path s_AssetPath;

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
		ImGui::Begin("Scene Hierarchy");

		static ImGuiTextFilter filter;
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, ImGui::GetFontSize() * 5);
		ImGui::Text("Search:");
		ImGui::NextColumn();
		filter.Draw("##filter", ImGui::GetContentRegionAvail().x);
		ImGui::Columns(1);
		ImGui::Separator();

		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

		if (ImGui::ListBoxHeader("##listbox 1", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
		{
			if (ImGui::IsMouseDown(0) && ImGui::IsItemHovered())
			{
				m_SelectionContext = {};
			}
			
			ItemRowsBackground(ImGui::GetFontSize());
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
					if (ImGui::BeginMenu("2D Objects"))
					{					
						if (ImGui::MenuItem("Square Object"))
						{
							Entity& newEntity = m_Context->CreateEntity("Square Object");
							newEntity.AddComponent<SpriteRendererComponent>();
						}
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}
			ImGui::ListBoxFooter();
		}
		ImGui::PopStyleColor();

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
			if (ImGui::MenuItem(std::string("Delete " + entity.GetComponent<TagComponent>().Tag).c_str()))
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
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

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
			
			if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
			{
				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();


		if (entity.HasComponent<TransformComponent>())
		{
			DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
				{
					DrawVec3Controls("Translation", component.Translation);
					glm::vec3 rotation = glm::degrees(component.Rotation);
					DrawVec3Controls("Rotation", rotation);
					component.Rotation = glm::radians(rotation);
					DrawVec3Controls("Scale", component.Scale, 1.0f);
				}, false);
		}
		if (entity.HasComponent<CameraComponent>())
		{
			DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
				{
					auto& camera = component.Camera;

					const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
					const char* currentProjectionTypeString = projectionTypeStrings[(int)component.Camera.GetProjectionType()];

					DrawCheckbox("Active", &component.Primary);

					ImGui::PushID("Projection");
					ImGui::Columns(2);
					ImGui::SetColumnWidth(0, 100.0f);
					ImGui::Text("Projection");

					ImGui::NextColumn();
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
					ImGui::PopID();
					ImGui::Columns(1);

					if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
					{
						float perspectiveFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
						if (DrawDragFloat("Vertical FOV", &perspectiveFOV))
							camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveFOV));

						float perspectiveNear = camera.GetPerspectiveNearClip();
						if (DrawDragFloat("Near", &perspectiveNear))
							camera.SetPerspectiveNearClip(perspectiveNear);

						float perspectiveFar = camera.GetPerspectiveFarClip();
						if (DrawDragFloat("Far", &perspectiveFar))
							camera.SetPerspectiveFarClip(perspectiveFar);
					}
					if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
					{
						float orthoSize = camera.GetOrthographicSize();
						if (DrawDragFloat("Size", &orthoSize))
							camera.SetOrthographicSize(orthoSize);

						float orthoNear = camera.GetOrthographicNearClip();
						if (DrawDragFloat("Near", &orthoNear))
							camera.SetOrthographicNearClip(orthoNear);

						float orthoFar = camera.GetOrthographicFarClip();
						if (DrawDragFloat("Far", &orthoFar))
							camera.SetOrthographicFarClip(orthoFar);

						DrawCheckbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
					}
				});
		}
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
			{
					DrawVec4ColorControls("Color", component.Color);
					DrawGap();
					DrawButton("Texture");
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
						{
							const wchar_t* path = (const wchar_t*)payload->Data;
							std::filesystem::path texturePath = std::filesystem::path(s_AssetPath) / path;
							component.Texture = Texture2D::Create(texturePath.string());
						}

						ImGui::EndDragDropTarget();
					}

					DrawDragFloat("Tiling Factor", &component.TilingFactor, 0.1f);
			});
		}
	}

}