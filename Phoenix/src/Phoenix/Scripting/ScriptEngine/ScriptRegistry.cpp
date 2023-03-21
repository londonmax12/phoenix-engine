#include "phxpch.h"
#include "ScriptRegistry.h"

#include "mono/jit/jit.h"

#include <Phoenix/Scripting/ScriptEngine/ScriptEngine.h>
#include <Phoenix/Input/Input.h>
#include <Phoenix/Application/Application.h>
#include <box2d/include/box2d/b2_body.h>

#define PHX_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Phoenix.InternalCalls::" #Name, Name)

namespace phx {

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;


	static bool Input_IsKeyDown(KeyCode key)
	{
		return Input::IsKeyPressed(key);
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}


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
	static void TransformComponent_GetRotation(UUID entityID, glm::vec3* outRotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);
		*outRotation = entity.GetComponent<TransformComponent>().Rotation;
	}
	static void TransformComponent_SetRotation(UUID entityID, glm::vec3* rotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);
		entity.GetComponent<TransformComponent>().Rotation = *rotation;
	}
	static void TransformComponent_GetScale(UUID entityID, glm::vec3* outScale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);
		*outScale = entity.GetComponent<TransformComponent>().Scale;
	}
	static void TransformComponent_SetScale(UUID entityID, glm::vec3* scale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);
		entity.GetComponent<TransformComponent>().Scale = *scale;
	}

	static void SpriteRenderComponent_GetColor(UUID entityID, glm::vec4* outColor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);
		glm::vec4 color = entity.GetComponent<SpriteRendererComponent>().Color;
		*outColor = color;
	}
	static void SpriteRenderComponent_SetColor(UUID entityID, glm::vec4* color)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);
		entity.GetComponent<SpriteRendererComponent>().Color = *color;
	}

	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}
	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}


	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
			{
				std::string_view typeName = typeid(Component).name();
				size_t pos = typeName.find_last_of(':');
				std::string_view structName = typeName.substr(pos + 1);
				std::string managedTypename = fmt::format("Phoenix.{}", structName);

				MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetAssemblyImage());
				if (!managedType)
				{
					PHX_CORE_ERROR("Could not find component type {}", managedTypename);
					return;
				}
				s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
			}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptRegistry::RegisterComponents()
	{
		RegisterComponent(AllComponents{});
	}


	void ScriptRegistry::RegisterFunctions()
	{
		PHX_ADD_INTERNAL_CALL(Input_IsKeyDown);

		PHX_ADD_INTERNAL_CALL(Entity_HasComponent);

		PHX_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		PHX_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		PHX_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		PHX_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		PHX_ADD_INTERNAL_CALL(TransformComponent_GetScale);
		PHX_ADD_INTERNAL_CALL(TransformComponent_SetScale);

		PHX_ADD_INTERNAL_CALL(SpriteRenderComponent_GetColor);
		PHX_ADD_INTERNAL_CALL(SpriteRenderComponent_SetColor);

		PHX_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		PHX_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);
	}
}
