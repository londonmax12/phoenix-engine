#include "phxpch.h"
#include "ContentBrowserPanel.h"
#include "ShaderEditorPanel.h"

#include <imgui.h>

namespace phx {
	extern const std::filesystem::path s_AssetPath = "assets";

	static float padding = 16.0f;
	static float thumbnailSize = 100.0f;
	float cellSize = thumbnailSize + padding;

	void ContentBrowserPanel::Refresh()
	{
		m_Files.clear();

		FileIcon newFileIcon;

		for (auto& itr : std::filesystem::directory_iterator(m_CurrentDirectory))
		{			
			newFileIcon.Path = itr;
			
			if (std::filesystem::is_directory(itr.path()))
			{
				if (std::filesystem::is_empty(itr.path()))
				{
					newFileIcon.FileType = FileType::DirEmpty;
				}
				else
				{
					newFileIcon.FileType = FileType::Dir;
				}
			}
			else
			{
				std::string extension = itr.path().extension().string();
				if (extension == ".png" || extension == ".jpg" || extension == ".bmp" || extension == ".jfif")
				{
					newFileIcon.FileType = FileType::ImageFile;
				}
				else if (extension == ".phoenix")
				{
					newFileIcon.FileType = FileType::PhoenixFile;
				}
				else
				{
					newFileIcon.FileType = FileType::OtherFile;
				}

			}

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
		m_PhoenixIcon = Texture2D::Create("resources/icons/content-browser/phoenix.png");

		m_RefreshIcon = Texture2D::Create("resources/icons/content-browser/refresh-icon.png");

		Refresh();
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		int size = ImGui::GetFontSize();
		if (ImGui::Button("<", ImVec2(size, size)))
		{
			if (m_CurrentDirectory != std::filesystem::path(s_AssetPath))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
				refresh = true;
			}		
		}
		ImGui::SameLine();
		if (ImGui::Button("+", ImVec2(size, size)))
		{
			std::string newDir = std::string(std::filesystem::absolute(m_CurrentDirectory).string() + "\\New Folder");
			int temp = mkdir(newDir.c_str());
			refresh = true;
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x - (size * 2));
		if (ImGui::ImageButton((ImTextureID)m_RefreshIcon->GetRendererID(), ImVec2(size, size)))
		{
			refresh = true;
		}
		ImGui::PopStyleColor();
		ImGui::Separator();

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& itr : m_Files)
		{	
			Ref<Texture2D> icon;
			switch (itr.FileType)
			{
			case FileType::Dir:
			{
				icon = m_DirectoryIcon;
				break;
			}
			case FileType::DirEmpty:
			{
				icon = m_DirectoryEmptyIcon;
				break;
			}
			case FileType::ImageFile:
			{
				icon = m_ImageIcon;
				break;
			}
			case FileType::PhoenixFile:
			{
				icon = m_PhoenixIcon;
				break;
			}
			default:
			{
				icon = m_FileIcon;
				break;
			}
			}
			
			auto relativePath = std::filesystem::relative(itr.Path, s_AssetPath);

			ImGui::PushID(itr.Path.c_str());
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (std::filesystem::is_directory(itr.Path))
					{
						m_CurrentDirectory /= itr.Path.filename();
						refresh = true;
					}
				}
			}
			ImGui::TextWrapped(itr.Path.filename().string().c_str());

			ImGui::NextColumn();
			ImGui::PopID();
		}


		ImGui::Columns(1);

		if (refresh)
		{
			Refresh();
			refresh = false;
		}

		ImGui::End();
	}
}