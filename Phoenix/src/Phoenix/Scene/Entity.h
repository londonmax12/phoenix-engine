#pragma once

#include "Phoenix/Scene/Scene.h"
#include "Phoenix/Scene/Components.h"
#include "Phoenix/Logging/Log.h"
#include "Phoenix/Application/UUID.h"

#include "entt.hpp"

namespace phx {
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			PHX_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			PHX_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent() 
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			PHX_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
		const std::string& GetName() { return GetComponent<TagComponent>().Tag; }

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

		/*void AddChild(Ref<Entity> entity) {
			m_Children.push_back(entity);
		}
		std::vector<Ref<Entity>> GetChildren() {
			return m_Children;
		}
		int ChilrenCount() {
			return m_Children.size();
		}
		bool IsChild() {
			if (m_Parent != nullptr)
				return true;
			else
				return false;
		}
		Ref<Entity> GetParent() {
			return m_Parent;
		}*/
	private:
		/*std::vector<Ref<Entity>> m_Children;
		Ref<Entity> m_Parent{};*/

		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene;
	};

}