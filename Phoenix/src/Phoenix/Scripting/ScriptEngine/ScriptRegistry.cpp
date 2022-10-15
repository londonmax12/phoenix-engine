#include "phxpch.h"
#include "ScriptRegistry.h"

#include "mono/jit/jit.h"

#include <Phoenix/Scripting/ScriptEngine/ScriptEngine.h>
#include <Phoenix/Input/Input.h>

#define PHX_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Phoenix.InternalCalls::" #Name, Name)

namespace phx {
	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);
		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);
		entity.GetComponent<TransformComponent>().Translation = *translation;
	}


	static bool Input_IsKeyDown(KeyCode key)
	{
		return Input::IsKeyPressed(key);
	}

	void ScriptRegistry::RegisterFunctions()
	{
		PHX_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		PHX_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		PHX_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}
