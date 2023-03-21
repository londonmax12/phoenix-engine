#include "phxpch.h"

#include "ContentBrowserPanel.h"
#include "ShaderEditorPanel.h"

#include "Phoenix/Utils/Filesystem.h"
#include "Phoenix/UI/GuiWidgets.h"

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
		std::string rootStr = root.string();
		std::string currentStr = current.string();

		if(rootStr[rootStr.length() - 1] == '/')
			rootStr.erase(rootStr.length() - 1);

		if (rootStr.find('/') != std::string::npos)
		{
			rootStr.erase(rootStr.find_last_of('/'));
			currentStr.replace(currentStr.find(rootStr), rootStr.length(), "");
		}

		if (currentStr[0] == '/')
		{
			currentStr.erase(0, 1);
			rootStr += '/';
		}
		if (currentStr[currentStr.length() - 1] != '/')
			currentStr += '/';

		while (currentStr.find("\\") != std::string::npos)
		{
			currentStr.replace(currentStr.find('\\'), 1, "/");
		}

		if (currentStr.find('/') != std::string::npos)
		{
			std::string path = std::string();
			for (auto& itr : currentStr)
			{
				path += itr;
				if (itr == '/')
				{
					parents.push_back(std::filesystem::path(rootStr + path));
				}
			}
		}
		else
		{
			parents.push_back(std::filesystem::path(rootStr + currentStr));	
		}
		return parents;
	}
	bool CompareFileIcon(const FileIcon& fi1,  const FileIcon& fi2)
	{
		if (fi1.FileType != fi2.FileType)
		{
			if (fi1.FileType == FileType::Dir)
				return true;
			else if (fi2.FileType == FileType::Dir)
				return false;

			if (fi1.FileType == FileType::DirEmpty)
				return true;
			else if (fi2.FileType == FileType::DirEmpty)
				return false;
		}
		return fi1.Path.string() < fi2.Path.string();
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
				else if (extension == ".wav" || extension == ".ogg" || extension == ".mp3")
				{
					newFileIcon.FileType = FileType::AudioFile;
				}
				else if (extension == ".phxobj")
				{
					newFileIcon.FileType = FileType::PrefabFile;
				}
				else
				{
					newFileIcon.FileType = FileType::OtherFile;
				}

			}

			m_Files.push_back(newFileIcon);
		}
		std::sort(m_Files.begin(), m_Files.end(), CompareFileIcon);
	}

	ContentBrowserPanel::ContentBrowserPanel()
	{
		m_Icons["dir_empty"] = Texture2D::Create("resources/icons/content-browser/directory-empty-icon.png");
		m_Icons["dir"] = Texture2D::Create("resources/icons/content-browser/directory-icon.png");

		m_Icons["default"] = Texture2D::Create("resources/icons/content-browser/file-icon.png");

		m_Icons["image_file"] = Texture2D::Create("resources/icons/content-browser/image-icon.png");
		m_Icons["scene_file"] = Texture2D::Create("resources/icons/content-browser/scene-icon.png");
		m_Icons["audio_file"] = Texture2D::Create("resources/icons/content-browser/audio-icon.png");
		m_Icons["prefab_file"] = Texture2D::Create("resources/icons/content-browser/prefab-icon.png");

		
		m_RefreshIcon = Texture2D::Create("resources/icons/content-browser/refresh-icon.png");

		m_AssetPath = s_AssetPath;
		m_CurrentDirectory = m_AssetPath;
		m_ParentDirs = GetParents(m_AssetPath, m_CurrentDirectory);

		Refresh();
	}

	ContentBrowserPanel::ContentBrowserPanel(std::string assetPath)
	{
		m_Icons["dir_empty"] = Texture2D::Create("resources/icons/content-browser/directory-empty-icon.png");
		m_Icons["dir"] = Texture2D::Create("resources/icons/content-browser/directory-icon.png");

		m_Icons["default"] = Texture2D::Create("resources/icons/content-browser/file-icon.png");

		m_Icons["image_file"] = Texture2D::Create("resources/icons/content-browser/image-icon.png");
		m_Icons["scene_file"] = Texture2D::Create("resources/icons/content-browser/scene-icon.png");
		m_Icons["audio_file"] = Texture2D::Create("resources/icons/content-browser/audio-icon.png");
		m_Icons["prefab_file"] = Texture2D::Create("resources/icons/content-browser/prefab-icon.png");


		m_RefreshIcon = Texture2D::Create("resources/icons/content-browser/refresh-icon.png");

		m_AssetPath = assetPath;
		m_CurrentDirectory = m_AssetPath;
		m_ParentDirs = GetParents(m_AssetPath, m_CurrentDirectory);

		Refresh();
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		float size = ImGui::GetFontSize();
		
		UI::PushHiddenButton();
		if (ImGui::Button("<", ImVec2(size, size)))
		{
			if (m_CurrentDirectory.string() + "/" != m_AssetPath)
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
				std::string name = itr.string();
				if (name.find('/') != std::string::npos)
				{
					name.erase(name.length() - 1);
					name = name.substr(name.find_last_of('/') + 1, name.length() - 1);
				}
				const char* nameChr = name.c_str();
				if (ImGui::Button(nameChr))
				{
					m_CurrentDirectory = itr;
					refresh = true;
					getparents = true;
				}
			}
			else
			{
				index++;
				ImGui::SameLine();
				std::string buttonName = itr.string();
				if (buttonName.find('/') != std::string::npos)
				{
					buttonName.erase(buttonName.find_last_of('/'));
					buttonName = buttonName.substr(buttonName.find_last_of('/') + 1, buttonName.length() - 1);
				}
				if (ImGui::Button(buttonName.c_str()))
				{
					m_CurrentDirectory = itr;
					refresh = true;
					getparents = true;
				}
			}
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
		UI::PopHiddenButton();
		ImGui::PopStyleColor();

		static ImGuiTextFilter filter;
		/*
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
		filter.Draw("Search ##content_filter");
		ImGui::PopItemWidth();*/

		ImGui::Separator();

		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
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
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

					switch (itr.FileType)
					{
					case FileType::Dir:
					{
						ImGui::ImageButton((ImTextureID)m_Icons["dir"]->GetRendererID(), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});
						break;
					}
					case FileType::DirEmpty:
					{
						ImGui::ImageButton((ImTextureID)m_Icons["dir_empty"]->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
						break;
					}
					case FileType::ImageFile:
					{
						ImGui::ImageButton((ImTextureID)m_Icons["image_file"]->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
						break;
					}
					case FileType::PhoenixFile:
					{
						ImGui::ImageButton((ImTextureID)m_Icons["scene_file"]->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
						break;
					}
					case FileType::MeshFile:
					{
						ImGui::ImageButton((ImTextureID)m_Icons["mesh_file"]->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
						break;
					}
					case FileType::AudioFile:
					{
						ImGui::ImageButton((ImTextureID)m_Icons["audio_file"]->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
						break;
					}
					default:
					{
						ImGui::ImageButton((ImTextureID)m_Icons["default"]->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
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
								getparents = true;
							}
						}
						auto relativePath = std::filesystem::relative(itr.Path, m_AssetPath);
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
							Refresh();
						}
						if (ImGui::MenuItem("Open in Explorer"))
						{
							Filesystem::OpenInFileExplorer(itr.Path.string());
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

					ImGui::PopStyleColor(3);
					ImGui::TextWrapped(itr.Path.filename().string().c_str());

					ImGui::NextColumn();
					ImGui::PopID();
				}
			}
			ImGui::Columns(1);

			ImGui::ListBoxFooter();
		}

		ImGui::PopStyleColor();

		if (getparents)
		{
			m_ParentDirs.clear();
			m_ParentDirs = GetParents(m_AssetPath, m_CurrentDirectory);
			getparents = false;
		}
		if (refresh)
		{
			Refresh();
			refresh = false;
		}


		ImGui::End();
	}
	void ContentBrowserPanel::UpdateAssetPath(std::string path)
	{
		m_AssetPath = path;
		m_CurrentDirectory = path;
		getparents = true;
		refresh = true;
		//m_ParentDirs = GetParents(m_AssetPath, m_CurrentDirectory);
	}
}