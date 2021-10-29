#pragma once
#include "entt.hpp"
#include "Phoenix/Time/DeltaTime.h"


namespace phx {
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string name = std::string());

		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(DeltaTime dt);
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
	};
}