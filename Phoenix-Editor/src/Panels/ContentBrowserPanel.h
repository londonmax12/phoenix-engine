#pragma once

#include <filesystem>

#include "Phoenix/Renderer/Texture.h"

namespace phx {
	struct FileIcon
	{
		FileIcon() = default;
		std::filesystem::path Path;
		Ref<Texture2D> icon;

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

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_DirectoryEmptyIcon;
		Ref<Texture2D> m_ImageIcon;
		Ref<Texture2D> m_FileIcon;

		std::vector<FileIcon> m_Files;
	};
}