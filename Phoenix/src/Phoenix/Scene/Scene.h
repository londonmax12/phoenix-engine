#pragma once

#include "Phoenix/Application/UUID.h"
#include "Phoenix/Renderer/EditorCamera.h"
#include "Phoenix/Time/DeltaTime.h"

#include <vector>
#include "../vendor/entt/include/entt.hpp"

class b2World;

namespace phx {
	class Entity;

	class Scene
	{
	private:
		entt::registry m_Registry;
	public:
		Scene();
		~Scene();

		enum class SceneType {
			Scene2D
		};

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntity(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		SceneType GetSceneType() { return m_SceneType; }
		void SetSceneType(SceneType type) { m_SceneType = type; }

		void SetGravity(float x, float y);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void Render2D();

		void UpdateScripts(DeltaTime dt);

		void OnUpdateRuntime(DeltaTime dt, Camera* camera, glm::mat4 cameraTransform);
		void OnUpdateEditor(DeltaTime dt, EditorCamera& camera);
		void OnUpdatePhysics(DeltaTime dt, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		void DuplicateEntity(Entity entity);

		size_t GetRegistrySize() { return m_Registry.size(); }
		entt::registry* GetRegistry() { return &m_Registry; }

		Entity GetPrimaryCameraEntity();

		Entity GetEntityByUUID(UUID uuid);



		template<typename... Components>
		auto GetAllEntitiesWith() { return m_Registry.view<Components...>(); }
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		
		float m_GravityX = 0.0f;
		float m_GravityY = -9.8f;

		b2World* m_PhysicsWorld = nullptr;

		SceneType m_SceneType = SceneType::Scene2D;

		std::unordered_map<UUID, entt::entity> m_EntityMap;
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}