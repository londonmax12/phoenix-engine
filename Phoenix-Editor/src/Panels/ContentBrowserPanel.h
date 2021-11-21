#pragma once

#include <filesystem>

#include "Phoenix/Renderer/Texture.h"

namespace phx {
	enum FileType {
		Dir, DirEmpty,
		PhoenixFile, ImageFile, ShaderFile,
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
	private:
		std::filesystem::path m_CurrentDirectory;

		bool refresh = false;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_DirectoryEmptyIcon;
		Ref<Texture2D> m_ImageIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_PhoenixIcon;

		Ref<Texture2D> m_RefreshIcon;

		std::vector<std::filesystem::path> m_ParentDirs;

		std::vector<FileIcon> m_Files;
	};
}