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
		{
			{
				m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
					{
						if (!nsc.Instance)
						{
							nsc.InstantiateFunction();
							nsc.Instance->m_Entity = Entity{ entity, this };

							if (nsc.OnCreateFunction)
								nsc.OnCreateFunction(nsc.Instance);
						}

						if (nsc.OnUpdateFunction)
							nsc.OnUpdateFunction(nsc.Instance, dt);
					});
			}
		}

		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}	

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuadFilled(transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}
}