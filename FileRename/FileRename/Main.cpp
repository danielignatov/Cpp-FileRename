#include "boost/filesystem.hpp"
#include <iostream>
#include <string>
#include <windows.h>

using namespace std;
using namespace boost::filesystem;

string getCurrentProgramFolder()
{
	vector<wchar_t> pathBuf;
	DWORD copied = 0;
	do {
		pathBuf.resize(pathBuf.size() + MAX_PATH);
		copied = GetModuleFileName(0, &pathBuf.at(0), pathBuf.size());
	} while (copied >= pathBuf.size());

	pathBuf.resize(copied);

	string path(pathBuf.begin(), pathBuf.end());

	return path; // TODO: Regex username with %userprofile% to workaround usernames
}

int main()
{
	// Current folder path
	path currentProgramPath = getCurrentProgramFolder();
	directory_iterator end_iter;

	// Print current exe location
	cout << getCurrentProgramFolder() << endl;

	// Store all file paths in folder
	vector<path> folderFilesPaths;
	
	try
	{
		if (exists(currentProgramPath) && is_directory(currentProgramPath))
		{
			for (directory_iterator dir_iter(currentProgramPath); dir_iter != end_iter; ++dir_iter)
			{
				if (is_regular_file(dir_iter->status()))
				{
					folderFilesPaths.push_back(dir_iter->path());
				}
			}
		}
	
		for each (path item in folderFilesPaths)
		{
			cout << item.string() << endl;
		}
	}
	catch (filesystem_error &e)
	{
		std::cerr << e.what() << '\n';
	}


	return 0;
}