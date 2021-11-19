#pragma once

#include "Phoenix/Scene/Scene.h"
#include "Phoenix/Application/Base.h"
#include "Phoenix/Scene/Entity.h"
#include "Phoenix/Renderer/Texture.h"
#include <imgui.h>

namespace phx {
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);
		void SetSelectedEntity() { m_SelectionContext = {}; };
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		Ref<Scene> m_Context;

		Ref<Texture2D> m_Alstolfo;

		Entity m_SelectionContext;
	};
}