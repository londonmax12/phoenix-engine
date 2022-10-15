#pragma once

#include <string>

namespace phx {
	class Filesystem {
	public:
		static void OpenInFileExplorer(std::string path);
		static char* ReadBytes(std::string filepath, uint32_t* outSize);
	};
}