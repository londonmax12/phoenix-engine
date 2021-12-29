#include "phxpch.h"

#include "ContentBrowserPanel.h"
#include "ShaderEditorPanel.h"

#include <imgui.h>

#include <atlbase.h>
#include <atlconv.h>

namespace phx {
	extern const std::filesystem::path s_AssetPath = "assets";

	static float padding = 16.0f;
	static float thumbnailSize = 100.0f;
	float cellSize = thumbnailSize + padding;

	std::vector<std::filesystem::path> GetParents(std::filesystem::path root, std::filesystem::path current)
	{
		std::vector<std::filesystem::path> parents;

		std::string path = std::string();
		for (auto x : current.string())
		{
			if (x == '\\')
			{
				parents.push_back(std::filesystem::path(path));
			}
			path += x;
		}
		if (path.empty())
		{
			parents.push_back(root);
		}
		else
		{
			parents.push_back(std::filesystem::path(path));
		}
		return parents;
	}

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
				else if (extension == ".phxscene")
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

		m_ParentDirs = GetParents(s_AssetPath, m_CurrentDirectory);

		Refresh();
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		static ImGuiTextFilter filter;
		filter.Draw("Search");

		ImGui::Separator();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		int size = ImGui::GetFontSize();
		bool getparents = false;
		if (ImGui::Button("<", ImVec2(size, size)))
		{
			if (m_CurrentDirectory != std::filesystem::path(s_AssetPath))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
				refresh = true;
				getparents = true;
			}		
		}
		
		ImGui::PopStyleColor();
		int index = 0;
		for (auto& itr : m_ParentDirs)
		{
			if (index != 0)
			{
				ImGui::SameLine();
				ImGui::Text("/");
				ImGui::SameLine();
				if (ImGui::Button(itr.filename().string().c_str()))
				{
					m_CurrentDirectory = itr;
					refresh = true;
					getparents = true;
				}
			}
			else
			{
				ImGui::SameLine();
				if (ImGui::Button(itr.filename().string().c_str()))
				{
					m_CurrentDirectory = itr;
					refresh = true;
					getparents = true;
				}
				index++;
			}
		}
		if (getparents)
		{
			m_ParentDirs.clear();
			m_ParentDirs = GetParents(s_AssetPath, m_CurrentDirectory);
			getparents = false;
		}

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

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

		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

		ImGui::Separator();

		if (ImGui::ListBoxHeader("##listbox 1", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 2)))
		{

			float panelWidth = ImGui::GetContentRegionAvail().x;
			int columnCount = (int)(panelWidth / cellSize);
			if (columnCount < 1)
				columnCount = 1;

			ImGui::Columns(columnCount, 0, false);


			for (auto& itr : m_Files)
			{
				if (filter.PassFilter(itr.Path.filename().string().c_str()))
				{

					ImGui::PushID(itr.Path.c_str());
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

					switch (itr.FileType)
					{
					case FileType::Dir:
					{
						ImGui::ImageButton((ImTextureID)m_DirectoryIcon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
						break;
					}
					case FileType::DirEmpty:
					{
						ImGui::ImageButton((ImTextureID)m_DirectoryEmptyIcon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
						break;
					}
					case FileType::ImageFile:
					{
						ImGui::ImageButton((ImTextureID)m_ImageIcon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
						break;
					}
					case FileType::PhoenixFile:
					{
						ImGui::ImageButton((ImTextureID)m_PhoenixIcon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
						break;
					}
					default:
					{
						ImGui::ImageButton((ImTextureID)m_FileIcon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
						break;
					}
					}

					if (ImGui::IsItemHovered())
					{
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							if (std::filesystem::is_directory(itr.Path))
							{
								m_CurrentDirectory /= itr.Path.filename();
								refresh = true;
								m_ParentDirs.clear();
								m_ParentDirs = GetParents(s_AssetPath, m_CurrentDirectory);
							}
						}
						auto relativePath = std::filesystem::relative(itr.Path, s_AssetPath);
						if (ImGui::BeginDragDropSource())
						{
							const wchar_t* itemPath = relativePath.c_str();
							ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
							ImGui::EndDragDropSource();
						}
					}

					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Duplicate"))
						{
							CA2W copyFile(itr.Path.string().c_str());

							std::string newFileStr = itr.Path.string() + " - Copy" + itr.Path.extension().string();
							CA2W newFile(newFileStr.c_str());

							CopyFile(copyFile, newFile, false);
						}
						/*if (ImGui::MenuItem("Rename"))
						{
							ImGui::OpenPopup("RenamePopup");
							std::string name = "Test";
							std::string newPath = std::string(std::filesystem::absolute(itr.Path.parent_path()).string() + "\\" + "Test" + itr.Path.extension().string());
							PHX_CORE_INFO(newPath);
							rename(itr.Path.string().c_str(), newPath.c_str());
						}*/
						ImGui::Separator();
						if (ImGui::MenuItem("Delete"))
						{
							remove(itr.Path.string().c_str());
							refresh = true;
						}
						ImGui::EndPopup();
					}

					ImGui::PopStyleColor();
					ImGui::TextWrapped(itr.Path.filename().string().c_str());

					ImGui::NextColumn();
					ImGui::PopID();
				}
			}
			ImGui::Columns(1);

			ImGui::ListBoxFooter();
		}

		ImGui::PopStyleColor();

		if (refresh)
		{
			Refresh();
			refresh = false;
		}


		ImGui::End();
	}
}