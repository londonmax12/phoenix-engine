#include "phxpch.h"
#include "Phoenix/Utils/PlatfromUtils.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Phoenix/Application/Application.h"

#include <commdlg.h>
#include <windows.h>
#include <shlobj.h>

namespace phx {
	std::string FileDialogs::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}
	std::string FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrDefExt = std::strchr(filter, '\0') + 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}

	std::string FileDialogs::GetDocumentsPath()
	{
		wchar_t* path = 0;
		SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_SIMPLE_IDLIST, NULL, &path);
		CoTaskMemFree(static_cast<void*>(path));
		std::wstring widestring(path);
		std::string pathStr(widestring.begin(), widestring.end());
		return pathStr;
	}
	std::string FileDialogs::BrowseFolder()
	{
		TCHAR path[MAX_PATH];

		//const char* path_param = saved_path.c_str();

		BROWSEINFO bi = { 0 };
		//bi.lpszTitle = ("Browse for folder...");
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
		//bi.lpfn = BrowseCallbackProc;
		//bi.lParam = (LPARAM)path_param;

		LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

		if (pidl != 0)
		{
			//get the name of the folder and put it in path
			SHGetPathFromIDList(pidl, path);

			//free memory used
			IMalloc* imalloc = 0;
			if (SUCCEEDED(SHGetMalloc(&imalloc)))
			{
				imalloc->Free(pidl);
				imalloc->Release();
			}

			std::wstring widestring(path);
			std::string pathStr(widestring.begin(), widestring.end());
			return pathStr;
		}

		return "";	
	}

	std::pair<int, int> phx::Hardware::GetDesktopResolution()
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		int horizontal = desktop.right;
		int vertical = desktop.bottom;
		return std::pair<int, int>(horizontal, vertical);
	}
}