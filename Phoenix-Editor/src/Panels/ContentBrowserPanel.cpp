#include "phxpch.h"
#include "ContentBrowserPanel.h"

#include <imgui.h>

namespace phx {
	static const std::filesystem::path s_AssetPath = "assets";

	static float padding = 16.0f;
	static float thumbnailSize = 100.0f;
	float cellSize = thumbnailSize + padding;

	void ContentBrowserPanel::Refresh()
	{
		m_Files.clear();

		for (auto& itr : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			FileIcon newFileIcon;
			newFileIcon.Path = itr;
			
			m_Files.push_back(newFileIcon);
		}
	}

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetPath)
	{
		m_DirectoryEmptyIcon = Texture2D::Create("resources/icons/content-browser/directory-empty-icon.png");
		m_DirectoryIcon = Texture2D::Create("resources/icons/content-browser/directory-icon.png");
		m_FileIcon = Texture2D::Create("resources/icons/content-browser/file-icon.png");
		m_ImageIcon = Texture2D::Create("resources/icons/content-browser/image-icon.png");

		Refresh();
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (ImGui::Button("<-"))
		{
			if (m_CurrentDirectory != std::filesystem::path(s_AssetPath))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}		
		}
		ImGui::SameLine();
		if (ImGui::Button("+"))
		{
			std::string newDir = std::string(std::filesystem::absolute(m_CurrentDirectory).string() + "\\New Folder");
			mkdir(newDir.c_str());
		}
		ImGui::Separator();

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);


		for (auto& itr : m_Files)
		{
			const auto& path = itr;
			auto relativePath = std::filesystem::relative(itr.Path, s_AssetPath);
			std::string filenameString = relativePath.filename().string();

			Ref<Texture2D> icon;

			if (std::filesystem::is_directory(itr.Path))
			{
				if (std::filesystem::is_empty(itr.Path))
				{
					icon = m_DirectoryEmptyIcon;
				}
				else
				{
					icon = m_DirectoryIcon;
				}
			}
			else
			{
				std::string extension = itr.Path.extension().string();
				if (extension == ".png" || extension == ".jpg")
				{
					icon = m_ImageIcon;
				}
				else
				{
					icon = m_FileIcon;
				}
				
			}
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (std::filesystem::is_directory(itr.Path))
					{
						m_CurrentDirectory /= itr.Path.filename();
					}
				}
			}

			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
		}


		ImGui::Columns(1);

		ImGui::End();
	}
}