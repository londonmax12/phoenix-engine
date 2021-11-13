#pragma once
#include <filesystem>
#include <Phoenix.h>
#include "ImGuiColorTextEdit/TextEditor.h"

namespace phx {
	class ShaderEditorPanel
	{
	public:
		ShaderEditorPanel();

		void SetContext(const std::filesystem::path context);

		void OnImGuiRender();

	private:
		void Save(std::filesystem::path path);
		void Apply();

		bool DoesCacheExist(std::filesystem::path path);
		void DeleteCache(std::filesystem::path path);

		std::filesystem::path m_Context;

		std::string m_PathStr = "None";
		TextEditor m_Editor;
	};
}