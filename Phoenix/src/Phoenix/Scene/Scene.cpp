#include "phxpch.h"
#include "Scene.h"
#include "glm/glm.hpp"

#include "Phoenix/Renderer/Renderer2D.h"

#include "Phoenix/Scene/Components.h"

namespace phx {
	Scene::Scene()
	{

		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
	}
	Scene::~Scene()
	{

	}
	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(DeltaTime dt)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuadFilled(transform, sprite.Color);
		}


	}
}