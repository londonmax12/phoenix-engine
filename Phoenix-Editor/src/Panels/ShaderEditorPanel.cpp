#include "ShaderEditorPanel.h"
#include <fstream>
#include "../EditorLayer.h"

namespace phx {
	extern const std::filesystem::path s_AssetPath;

	ShaderEditorPanel::ShaderEditorPanel()
	{
		auto lang = TextEditor::LanguageDefinition::GLSL();

		m_Editor.SetLanguageDefinition(lang);
		m_Editor.SetShowWhitespaces(false);
	}
	void ShaderEditorPanel::SetContext(const std::filesystem::path context)
	{
		m_Context = context;

		m_PathStr = m_Context.filename().string();

		std::ifstream t;
		t.open(m_Context);
		if (t.good())
		{
			std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			m_Editor.SetText(str);
		}
		else
		{
			PHX_CORE_WARN("Failed to open shader: {0}", m_Context.string());
		}
	}
	void ShaderEditorPanel::OnImGuiRender()
	{
		ImGui::Begin("Shader Editor");

		if (ImGui::Button("Apply"))
		{
			Apply();
		}
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			Save(m_Context);
		}

		auto cpos = m_Editor.GetCursorPosition();
		ImGui::Text("%6d/%-6d %6d lines  | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, m_Editor.GetTotalLines(),
			m_Editor.GetLanguageDefinition().mName.c_str(), m_PathStr.c_str());
		ImGui::Separator();
		m_Editor.Render("Shader Editor");
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;

				std::filesystem::path fsPath = std::filesystem::path(path);
				std::string ext = fsPath.extension().string();

				if (ext == ".vert" || ext == ".frag")
				{
					SetContext(std::filesystem::path("assets") / fsPath);
				}
				else
				{
					PHX_CORE_WARN("Not a valid shader file: {0}", fsPath.string());
				}
			}
		}
		ImGui::End();
	}
	void ShaderEditorPanel::Save(std::filesystem::path path)
	{
		std::ofstream file;
		file.open(path);
		file << m_Editor.GetText();
		file.close();
	}
	void ShaderEditorPanel::Apply()
	{
		Save(m_Context);
		DeleteCache(m_Context.stem().string() + ".vert");
		DeleteCache(m_Context.stem().string() + ".frag");

		Shader::Create(m_Context.stem().string(), m_Context.stem().string() + ".vert", m_Context.stem().string() + ".frag");
	}
	bool ShaderEditorPanel::DoesCacheExist(std::filesystem::path path)
	{
		std::ifstream file;
		std::string vert = ("assets/cache/shader/opengl/" + path.string() + ".cached_opengl.vert");
		file.open(vert);
		if (file.good())
		{
			return true;
		}
		else
		{

			return false;
		}
	}
	void ShaderEditorPanel::DeleteCache(std::filesystem::path path)
	{
		if (DoesCacheExist(path))
		{
			std::string glvert = ("assets/cache/shader/opengl/" + path.string() + ".cached_opengl.vert");
			std::string glfrag = ("assets/cache/shader/opengl/" + path.string() + ".cached_opengl.frag");
			std::string vulkanvert = ("assets/cache/shader/opengl/" + path.string() + ".cached_vulkan.vert");
			std::string vulkanfrag = ("assets/cache/shader/opengl/" + path.string() + ".cached_vulkan.frag");

			remove(glvert.c_str());
			remove(glfrag.c_str());
			remove(vulkanvert.c_str());
			remove(vulkanfrag.c_str());
		}
		else
		{
			return;
		}
	}
}