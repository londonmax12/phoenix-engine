#pragma once

#include "Phoenix/Application/Core.h"
#pragma warning(push, 0) 
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#pragma warning(pop)

namespace phx {
	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; };
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; };
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

#ifdef PHX_DEBUG_MODE
#define PHX_CORE_FATAL(...)::phx::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define PHX_CORE_ERROR(...)::phx::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PHX_CORE_WARN(...)::phx::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PHX_CORE_INFO(...)::phx::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PHX_CORE_TRACE(...)::phx::Log::GetCoreLogger()->trace(__VA_ARGS__)

#define PHX_CLIENT_FATAL(...)::phx::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define PHX_CLIENT_ERROR(...)::phx::Log::GetClientLogger()->error(__VA_ARGS__)
#define PHX_CLIENT_WARN(...)::phx::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PHX_CLIENT_INFO(...)::phx::Log::GetClientLogger()->info(__VA_ARGS__)
#define PHX_CLIENT_TRACE(...)::phx::Log::GetClientLogger()->trace(__VA_ARGS__)
#else 
#define PHX_CORE_FATAL
#define PHX_CORE_ERROR
#define PHX_CORE_WARN
#define PHX_CORE_INFO
#define PHX_CORE_TRACE

#define PHX_CLIENT_FATAL
#define PHX_CLIENT_ERROR
#define PHX_CLIENT_WARN
#define PHX_CLIENT_INFO
#define PHX_CLIENT_TRACE
#endif