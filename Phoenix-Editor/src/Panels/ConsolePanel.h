#pragma once

#include <vector>
#include <chrono>
#include <string>

namespace phx {
	class ConsolePanel
	{
	public:
		ConsolePanel();
		
		enum class LogType 
		{
			trace = 0, info = 1, warn = 2, error = 3, fatal = 4
		};

		struct Log 
		{
			LogType Type;
			std::string Message;
			std::string Time;
		};

		void Trace(std::string message);
		void Info(std::string message);
		void Warn(std::string message);
		void Error(std::string message);
		void Fatal(std::string message);

		void AddLog(LogType type, std::string message);
		void OnImGuiRender();

	private:
		std::vector<Log> m_Logs;

		bool Scroll = false;
	};
}