#pragma once

#include "Phoenix/Renderer/Texture.h"

#include <filesystem>
#include <map>
#include <string>

namespace phx {
	enum FileType {
		Dir, DirEmpty,
		PhoenixFile, ImageFile, AudioFile, MeshFile, PrefabFile,
		OtherFile
	};
	struct FileIcon
	{
		FileIcon() = default;

		std::filesystem::path Path;
		FileType FileType;

		operator std::filesystem::path() const { return Path; }
	};

	class ContentBrowserPanel
	{
	public:		
		ContentBrowserPanel();
		void Refresh();
		void OnImGuiRender();	

		void UpdateAssetPath(std::string path);

		std::filesystem::path m_AssetPath;
	private:
		std::filesystem::path m_CurrentDirectory;

		bool refresh = false;

		std::map<std::string, Ref<Texture2D>> m_Icons;

		Ref<Texture2D> m_RefreshIcon;

		std::vector<std::filesystem::path> m_ParentDirs;

		std::vector<FileIcon> m_Files;
	};
}