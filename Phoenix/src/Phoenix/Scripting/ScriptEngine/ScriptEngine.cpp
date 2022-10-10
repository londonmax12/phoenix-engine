#include "phxpch.h"
#include "ScriptEngine.h"

#include "mono/metadata/assembly.h"

namespace phx {
	void ScriptEngine::Init()
	{
		if (!InitMono())
			PHX_CORE_ERROR("Failed to init scripting engine");
	}
	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
	}

	void ScriptEngine::PrintAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		PHX_CORE_TRACE("Scripting Assembly Types:");
		for (int32_t i = 0; i < numTypes; i++) {
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);
			
			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			PHX_CORE_TRACE("\t{0}.{1}", nameSpace, name);
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
		PrintAssemblyTypes(m_CoreAssembly);

		return true;
	}
	void ScriptEngine::ShutdownMono()
	{
		mono_domain_unload(m_AppDomain);
		mono_jit_cleanup(m_RootDomain);

		m_AppDomain = nullptr;
		m_RootDomain = nullptr;
	}
	char* ScriptEngine::ReadBytes(std::string filepath, uint32_t* outSize)
	{
		std::ifstream stream{ filepath, std::ios::binary | std::ios::ate };
		if (!stream) {
			PHX_CORE_ERROR("Failed to read bytes from file: {0}", filepath);
			return nullptr;
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();

		if (size == 0) {
			return nullptr;
		}

		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
	}
	MonoAssembly* ScriptEngine::LoadCSharpAssembly(std::string assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);

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
}