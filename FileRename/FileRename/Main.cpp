#include "boost/filesystem.hpp"
#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <regex>
#include <ctype.h>
#include <locale.h>

using namespace boost::filesystem;

std::locale loc("");

std::string checkAndFixFileName(std::string fileName)
{
	std::string fixedFileName = "";
	char previousChar = '*';
	char currentChar = '*';

	for (unsigned int i = 0; i < fileName.length(); i++)
	{
		currentChar = fileName[i];

		// If first character is lower make him upper
		if ((i == 0) && (islower(currentChar, loc)))
		{
			fixedFileName += toupper(currentChar);
			previousChar = toupper(currentChar);
			continue;
		}
		// If previous character is upper and current character is upper make current lower
		// OR
		// If previous character is lower and current character is upper make current lower
		else if (((isupper(previousChar, loc)) && (isupper(currentChar, loc))) || ((islower(previousChar, loc)) && (isupper(currentChar, loc))))
		{
			fixedFileName += tolower(currentChar);
			previousChar = tolower(currentChar);
			continue;
		}
		// If current character is not last
		else if (i != fileName.length() - 1)
		{
			char nextChar = fileName[i + 1];

			// If current character is upper and next character is space make current character lower
			if ((isupper(currentChar, loc)) && (nextChar == ' '))
			{
				fixedFileName += tolower(currentChar);
				previousChar = tolower(currentChar);
				continue;
			}
			// If current character is space and next character is lower make next upper and skip iteration
			else if ((currentChar == ' ') && (islower(nextChar, loc)))
			{
				fixedFileName += currentChar;
				fixedFileName += toupper(nextChar);
				previousChar = fileName[i + 1];
				i += 1;
				continue;
			}
			// If current character is space and next character is space do nothing
			else if ((currentChar == ' ') && (nextChar == ' '))
			{
				previousChar = fileName[i];
				continue;
			}
		}

		fixedFileName += currentChar;
		previousChar = fileName[i];
	}

	return fixedFileName;
}

int main(int argc, char *argv[])
{
	// Setting locale	
	std::locale::global(loc);  // (*)
	std::wcout.imbue(std::locale());
	auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());

	// Current folder path
	path currentProgramExecutablePath = argv[0];
	path currentProgramFolderPath = currentProgramExecutablePath.remove_filename();

	directory_iterator end_iter;

	// Store all file paths in folder
	std::vector<path> folderFilesPaths;
	std::vector<path> newFolderFilesPaths;

	// Regex. Group 1: File name
	std::regex getFileNameRegex("(.+\\\\+)(.+)(\\..+)");

	try
	{
		// Check if current path exists and it's directory.
		if (exists(currentProgramFolderPath) && is_directory(currentProgramFolderPath))
		{
			// For each regular file put the path to the vector<path>
			for (directory_iterator dir_iter(currentProgramFolderPath); dir_iter != end_iter; ++dir_iter)
			{
				if ((is_regular_file(dir_iter->status())))
				{
					folderFilesPaths.push_back(dir_iter->path());
				}
			}
		}

		// Write future paths
		for each (path item in folderFilesPaths)
		{
			std::string pathAsString = item.string();
			std::smatch getFileNameMatch;
			std::regex_search(pathAsString, getFileNameMatch, getFileNameRegex);

			// getFileNameMatch[1] - Path before file name
			// getFileNameMatch[2] - File name
			// getFileNameMatch[3] - File name extension

			std::string newFilePathAsString;
			newFilePathAsString += getFileNameMatch[1];
			newFilePathAsString += checkAndFixFileName(getFileNameMatch[2]);
			newFilePathAsString += getFileNameMatch[3];
			newFolderFilesPaths.push_back(newFilePathAsString);
		}

		// Rename files in current folder
		for (unsigned int i = 0; i < folderFilesPaths.size(); i++)
		{
			rename(folderFilesPaths[i], newFolderFilesPaths[i]);
			std::cout << newFolderFilesPaths[i] << std::endl;
		}
	}
	catch (filesystem_error &e)
	{
		std::cerr << e.what() << '\n';
	}

	return 0;
}