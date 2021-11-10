#pragma once

#include "Phoenix/Application/UUID.h"
#include "Phoenix/Renderer/EditorCamera.h"
#include "Phoenix/Time/DeltaTime.h"

#include "entt.hpp"

class b2World;

namespace phx {
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		enum class SceneType {
			Scene2D, Scene3D
		};

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntity(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		SceneType GetSceneType() { return m_SceneType; }
		void SetSceneType(SceneType type) { m_SceneType = type; }

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateRuntime(DeltaTime dt);
		void OnUpdateEditor(DeltaTime dt, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		void DuplicateEntity(Entity entity);

		uint32_t GetRegistrySize() { return m_Registry.size(); }

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;

		SceneType m_SceneType;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}