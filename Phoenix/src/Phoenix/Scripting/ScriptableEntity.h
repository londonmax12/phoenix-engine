#pragma once
#include "Phoenix/Scene/Entity.h"

namespace phx {
	class ScriptableEntity
	{
	public:
		ScriptableEntity() 
		{

		}

		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnUpdate(DeltaTime dt) {}
		virtual void OnDestroy() {}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}