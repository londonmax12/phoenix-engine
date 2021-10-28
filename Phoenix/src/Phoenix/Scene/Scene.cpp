#include "phxpch.h"
#include "Scene.h"
#include "glm/glm.hpp"

#include "Phoenix/Renderer/Renderer2D.h"

#include "Phoenix/Scene/Components.h"
#include "Phoenix/Scene/Entity.h"

namespace phx {
	Scene::Scene()
	{

		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
	}
	Scene::~Scene()
	{

	}
	Entity Scene::CreateEntity(const std::string name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
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