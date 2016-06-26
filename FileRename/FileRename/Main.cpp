#include "boost/filesystem.hpp"
#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <regex>
#include <ctype.h>

using namespace std;
using namespace boost::filesystem;

string checkAndFixFileName(string fileName)
{
	string fixedFileName;

	for (unsigned int i = 0; i < fileName.length(); i++)
	{
		char previousChar;
		char currentChar = fileName[i];

		if ((i == 0) && (islower(currentChar)))
		{
			fixedFileName += toupper(currentChar);
			previousChar = toupper(currentChar);
			continue;
		}
		else if ((isupper(previousChar)) && (isupper(currentChar)))
		{
			fixedFileName += tolower(currentChar);
			previousChar = tolower(currentChar);
			continue;
		}
	}

	return fixedFileName;
}

int main(int argc, char *argv[])
{
	// Current folder path
	path currentProgramExecutablePath = argv[0];
	path currentProgramFolderPath = currentProgramExecutablePath.remove_filename();

	directory_iterator end_iter;

	// Store all file paths in folder
	vector<path> folderFilesPaths;

	// Regex. Group 1: File name
	regex getFileNameRegex("(.+\\\\+)(.+)(\\..+)");

	try
	{
		// Check if current path exists and it's directory.
		if (exists(currentProgramFolderPath) && is_directory(currentProgramFolderPath))
		{
			// For each regular file put the path to the vector<path>
			for (directory_iterator dir_iter(currentProgramFolderPath); dir_iter != end_iter; ++dir_iter)
			{
				if (is_regular_file(dir_iter->status()))
				{
					folderFilesPaths.push_back(dir_iter->path());
				}
			}
		}

		// Print current collected paths
		for each (path item in folderFilesPaths)
		{
			cout << item.string() << endl;
			string pathAsString = item.string();
			smatch getFileNameMatch;
			regex_search(pathAsString, getFileNameMatch, getFileNameRegex);

			cout << getFileNameMatch[2] << endl;


		}
	}
	catch (filesystem_error &e)
	{
		std::cerr << e.what() << '\n';
	}


	return 0;
}