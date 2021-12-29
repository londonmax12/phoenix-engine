#pragma once

#include <string>

namespace phx {
	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);

		static std::string GetDocumentsPath();

		static std::string BrowseFolder();
	};
	
	class Hardware
	{
	public:
		static std::pair<int, int> GetDesktopResolution();
	};
}