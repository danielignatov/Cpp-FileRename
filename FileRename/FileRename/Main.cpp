#include "boost/filesystem.hpp"
#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <regex>
#include <ctype.h>
#include <locale.h>

using namespace std;
using namespace boost::filesystem;

string checkAndFixFileName(string fileName)
{
	string fixedFileName = "";
	char previousChar = '*';
	char currentChar = '*';

	for (unsigned int i = 0; i < fileName.length(); i++)
	{
		currentChar = fileName[i];

		// If first character is lower make him upper
		if ((i == 0) && (islower(currentChar)))
		{
			fixedFileName += toupper(currentChar);
			previousChar = toupper(currentChar);
			continue;
		}
		// If previous character is upper and current character is upper make current lower
		// OR
		// If previous character is lower and current character is upper make current lower
		else if (((isupper(previousChar)) && (isupper(currentChar))) || ((islower(previousChar)) && (isupper(currentChar))))
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
			if ((isupper(currentChar)) && (nextChar == ' '))
			{
				fixedFileName += tolower(currentChar);
				previousChar = tolower(currentChar);
				continue;
			}
			// If current character is space and next character is lower make next upper and skip iteration
			else if ((currentChar == ' ') && (islower(nextChar)))
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
	std::locale::global(std::locale(""));  // (*)
	std::wcout.imbue(std::locale());
	auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());

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

		// Test method
		// It break here when i enter cirylic letters, need to add better isupper islowr maybe
		std::string a = checkAndFixFileName("ÀÄÄÄÄÄÄ      äåëèâåÐ");
		std::cout << a;
	}
	catch (filesystem_error &e)
	{
		std::cerr << e.what() << '\n';
	}

	return 0;
}