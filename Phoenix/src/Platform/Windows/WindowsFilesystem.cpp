#include "phxpch.h"

#include "Phoenix/Utils/Filesystem.h"

#include <shellapi.h>
#include <filesystem>

namespace phx {
	void Filesystem::OpenInFileExplorer(std::string path) {
		std::filesystem::path fsPath(path);
		ShellExecuteA(NULL, "open", fsPath.parent_path().string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
	}

	char* Filesystem::ReadBytes(std::string filepath, uint32_t* outSize)
	{
		std::ifstream stream{ filepath, std::ios::binary | std::ios::ate };
		if (!stream) {
			PHX_CORE_ERROR("Failed to read bytes from file: {0}", filepath);
			return nullptr;
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();

		if (size == 0) {
			return nullptr;
		}

		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
	}
}