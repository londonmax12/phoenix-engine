#include "phxpch.h"

#include "Phoenix/Utils/Filesystem.h"

#include <shellapi.h>
#include <filesystem>

namespace phx {
	void Filesystem::OpenInFileExplorer(std::string path) {
		std::filesystem::path fsPath(path);
		ShellExecuteA(NULL, "open", fsPath.parent_path().string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
	}
}