#pragma once

#include <string>

#include "mono/jit/jit.h"

namespace phx {
	class ScriptEngine {
	public:
		static void Init();
		static void Shutdown();

		static void PrintAssemblyTypes(MonoAssembly* assembly);
	private:
		static bool InitMono();
		static void ShutdownMono();

		static char* ReadBytes(std::string filepath, uint32_t* outSize);
		static MonoAssembly* LoadCSharpAssembly(std::string assemblyPath);

		inline static MonoDomain* m_RootDomain;
		inline static MonoDomain* m_AppDomain;

		inline static MonoAssembly* m_CoreAssembly;
	};
}