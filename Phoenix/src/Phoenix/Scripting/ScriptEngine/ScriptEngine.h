#pragma once

#include <string>

#include "ScriptRegistry.h"

#include "mono/jit/jit.h"

#include <Phoenix/Scene/Scene.h>
#include <Phoenix/Scene/Entity.h>
#include <Phoenix/Scene/Components.h>
namespace phx {
	class ScriptClass {
	public:
		ScriptClass(const std::string& classNamespace, const std::string& className, MonoImage* image)
			: m_ClassName(className), m_ClassNamespace(classNamespace)
		{
			m_MonoClass = mono_class_from_name(image, classNamespace.c_str(), className.c_str());
		}

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int paramCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;

		MonoClass* m_MonoClass = nullptr;
	};

	class ScriptInstance {
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, ScriptClass* entityClass, Entity entity);
		
		void InvokeOnCreate();
		void InvokeOnUpdate(float deltaTime);

	private:
		Ref<ScriptClass> m_ScriptClass;
		
		MonoObject* m_Instance = nullptr;
		MonoMethod* m_ConstructorMethod = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;
	};

	class ScriptEngine {
	public:
		static void Init();
		static void Shutdown();

		static MonoAssembly* LoadCSharpAssembly(std::string assemblyPath);
		static void LoadCSharpAssemblyClasses(MonoAssembly* assembly);

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		
		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static bool DoesClassExist(const std::string& fullClassName);

		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, float deltaTime);

		static Scene* GetSceneContext() { return m_SceneContext; }

	private:
		static bool InitMono();
		static void ShutdownMono();

		inline static MonoDomain* m_RootDomain = nullptr;
		inline static MonoDomain* m_AppDomain = nullptr;

		inline static MonoAssembly* m_CoreAssembly = nullptr;
		inline static MonoImage* m_CoreAssemblyImage = nullptr;

		inline static MonoAssembly* m_AppAssembly = nullptr;
		inline static MonoImage* m_AppAssemblyImage = nullptr;

		inline static ScriptClass* m_EntityClass = nullptr;

		static inline std::unordered_map<std::string, Ref<ScriptClass>> m_EntityClasses;
		static inline std::unordered_map<UUID, Ref<ScriptInstance>> m_EntityInstances;

		inline static Scene* m_SceneContext;
	};
}