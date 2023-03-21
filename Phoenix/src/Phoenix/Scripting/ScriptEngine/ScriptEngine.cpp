#include "phxpch.h"
#include "ScriptEngine.h"

#include "Phoenix/Utils/Filesystem.h"

#include "mono/metadata/assembly.h"


namespace phx {

	void ScriptEngine::Init()
	{
		if (!InitMono())
		{
			PHX_CORE_ERROR("Failed to init scripting engine");
			return;
		}

		ScriptRegistry::RegisterFunctions();
		ScriptRegistry::RegisterComponents();

		m_EntityClass = new ScriptClass("Phoenix", "Entity", m_CoreAssemblyImage);
		MonoObject* instance = m_EntityClass->Instantiate();
	}

	void ScriptEngine::Shutdown()
	{
		delete m_EntityClass;
		ShutdownMono();
	}

	void ScriptEngine::LoadCSharpAssemblyClasses(MonoAssembly* assembly)
	{
		m_EntityClasses.clear();

		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		MonoClass* entityClass = mono_class_from_name(m_CoreAssemblyImage, "Phoenix", "Entity");

		PHX_CORE_TRACE("Scripting Assembly Types:");
		for (int32_t i = 0; i < numTypes; i++) {
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);
			
			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, name);
			else
				fullName = name;

			MonoClass* monoClass = mono_class_from_name(image, nameSpace, name);

			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);

			if (isEntity) {
				m_EntityClasses[fullName] = CreateRef<ScriptClass>(nameSpace, name, m_AppAssemblyImage);
			}

			PHX_CORE_TRACE("\t{}", fullName);
		}
	}
	bool ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		m_RootDomain = mono_jit_init("PhoenixJITRuntime");
		if (m_RootDomain == nullptr)
		{
			PHX_CORE_ERROR("Failed to init mono root domain");
			return false;
		}

		m_AppDomain = mono_domain_create_appdomain("PhoenixScriptRuntime", nullptr);
		if (m_AppDomain == nullptr)
		{
			PHX_CORE_ERROR("Failed to create mono app domain");
			return false;
		}
		mono_domain_set(m_AppDomain, true);

		m_CoreAssembly = LoadCSharpAssembly("resources/scripts/Phoenix-Script.dll");
		m_CoreAssemblyImage = mono_assembly_get_image(m_CoreAssembly);

		m_AppAssembly = LoadCSharpAssembly("../ExampleProject/Assets/Scripts/bin/Sandbox.dll");
		m_AppAssemblyImage = mono_assembly_get_image(m_AppAssembly);

		LoadCSharpAssemblyClasses(m_AppAssembly);

		return true;
	}
	void ScriptEngine::ShutdownMono()
	{
		mono_domain_unload(m_AppDomain);
		mono_jit_cleanup(m_RootDomain);

		m_AppDomain = nullptr;
		m_RootDomain = nullptr;
	}

	MonoAssembly* ScriptEngine::LoadCSharpAssembly(std::string assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = Filesystem::ReadBytes(assemblyPath, &fileSize);

		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		if (status != MONO_IMAGE_OK) {
			const char* errorMsg = mono_image_strerror(status);
			PHX_CORE_ERROR("Failed to load mono image while loading C# assembly: {0}", errorMsg);
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		delete[] fileData;

		return assembly;
	}
	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(m_AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}
	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		m_SceneContext = scene;
	}
	void ScriptEngine::OnRuntimeStop()
	{
		m_SceneContext = nullptr;

		m_EntityInstances.clear();
	}
	bool ScriptEngine::DoesClassExist(const std::string& fullClassName)
	{
		return m_EntityClasses.find(fullClassName) != m_EntityClasses.end();
	}
	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		auto& sc = entity.GetComponent<ScriptComponent>();
		if (ScriptEngine::DoesClassExist(sc.Name)) {
			Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(m_EntityClasses[sc.Name], m_EntityClass, entity);
			m_EntityInstances[entity.GetUUID()] = instance;
			instance->InvokeOnCreate();
		}
	}
	void ScriptEngine::OnUpdateEntity(Entity entity, float deltaTime)
	{
		Ref<ScriptInstance> instance = m_EntityInstances[entity.GetUUID()];
		instance->InvokeOnUpdate(deltaTime);
	}
	MonoObject* ScriptClass::Instantiate()
	{
		return  ScriptEngine::InstantiateClass(m_MonoClass);
	}
	MonoMethod* ScriptClass::GetMethod(const std::string& name, int paramCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, name.c_str(), paramCount);
	}
	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, ScriptClass* entityClass, Entity entity)
		: m_ScriptClass(scriptClass)
	{
		m_Instance = scriptClass->Instantiate();

		m_ConstructorMethod = entityClass->GetMethod(".ctor", 1);
		m_OnCreateMethod = scriptClass->GetMethod("OnCreate", 0);
		m_OnUpdateMethod = scriptClass->GetMethod("OnUpdate", 1);

		UUID uuid = entity.GetUUID();
		void* param = &uuid;
		m_ScriptClass->InvokeMethod(m_Instance, m_ConstructorMethod, &param);
	}
	void ScriptInstance::InvokeOnCreate()
	{
		m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod);
	}
	void ScriptInstance::InvokeOnUpdate(float deltaTime)
	{
		void* param = &deltaTime;
		m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, &param);
	}
}