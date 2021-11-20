#include "ConsolePanel.h"

#include <sstream>
#include <ctime>
#include <iomanip>

#include <imgui.h>

namespace phx {
	using time_point = std::chrono::system_clock::time_point;
	std::string SerializeTimePoint(const time_point& time, const std::string& format)
	{
		std::time_t tt = std::chrono::system_clock::to_time_t(time);
		//std::tm tm = *std::gmtime(&tt); //GMT (UTC)
		std::tm tm = *std::localtime(&tt); //Locale time-zone, usually UTC by default.
		std::stringstream ss;
		ss << std::put_time(&tm, format.c_str());
		return ss.str();
	}

	ConsolePanel::ConsolePanel()
	{
	}

	void ConsolePanel::Trace(std::string message)
	{
		AddLog(LogType::trace, message);
	}

	void ConsolePanel::Info(std::string message)
	{
		AddLog(LogType::info, message);
	}

	void ConsolePanel::Warn(std::string message)
	{
		AddLog(LogType::warn, message);
	}

	void ConsolePanel::Error(std::string message)
	{
		AddLog(LogType::error, message);
	}

	void ConsolePanel::Fatal(std::string message)
	{
		AddLog(LogType::fatal, message);
	}

	void ConsolePanel::AddLog(LogType type, std::string message)
	{
		Log newLog;
		newLog.Message = message;
		newLog.Type = type;
		time_point tp = std::chrono::system_clock::now();
		std::string time = SerializeTimePoint(tp, "%H:%M:%S");
		newLog.Time = time;
		if (m_Logs.size() > 300)
		{
			m_Logs.erase(m_Logs.begin());
		}
		m_Logs.push_back(newLog);
		Scroll = true;
	}

	void ConsolePanel::OnImGuiRender()
	{
		if (ImGui::Begin("Console"))
		{
			static ImGuiTextFilter filter;
			filter.Draw("##filter");
			ImGui::SameLine();
			if (ImGui::Button("Clear"))
			{
				m_Logs.clear();
			}
			ImGui::SameLine();
			ImGui::Text("Logs: %i", m_Logs.size());
			if (ImGui::ListBoxHeader("##listbox 1", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 2)))
			{
				for (auto& itr : m_Logs)
				{
					const char* message = itr.Message.c_str();
					const char* time = itr.Time.c_str();

					if (filter.PassFilter(message))
					{
						switch (itr.Type)
						{
						case LogType::trace:
						{
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8, 0.8, 0.8, 1.0f));
							ImGui::Text("[Trace] %s:", time);
							ImGui::PopStyleColor();
							ImGui::SameLine();
							ImGui::Text("%s", message);
							break;
						}
						case LogType::info:
						{
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2, 0.7, 0.1, 1.0f));
							ImGui::Text("[Info] %s:", time);
							ImGui::PopStyleColor();
							ImGui::SameLine();
							ImGui::Text("%s", message);
							break;
						}
						case LogType::warn:
						{
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8, 0.8, 0.1, 1.0f));
							ImGui::Text("[Warning] %s:", time);
							ImGui::PopStyleColor();
							ImGui::SameLine();
							ImGui::Text("%s", message);
							break;
						}

						case LogType::error:
						{
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8, 0.2, 0.1, 1.0f));
							ImGui::Text("[Error] %s:", time);
							ImGui::PopStyleColor();
							ImGui::SameLine();
							ImGui::Text("%s", message);
							break;
						}
						case LogType::fatal:
						{
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4, 0.0, 0.0, 1.0f));
							ImGui::Text("[Fatal] %s:", time);
							ImGui::PopStyleColor();
							ImGui::SameLine();
							ImGui::Text("%s", message);
							break;
						}
						default:
							break;
						}
					}
				}
				if (Scroll)
				{
					ImGui::SetScrollHereY(0.0f);
					Scroll = false;
				}
				ImGui::ListBoxFooter();
			}
		}
		ImGui::End();
	}
}

