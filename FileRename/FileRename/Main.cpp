#include <boost/filesystem.hpp>
#include <iostream>

using namespace std;
using namespace boost::filesystem;

int main()
{
	// Current folder path
	path p{ "C:\\test" }; // TODO: Change on release - boost::filesystem::system_complete(argv[0]); - you need to remove own program name from it

	// Store all file paths in folder
	vector<path> folderFilesPaths;

	try
	{

		//if (create_directory(p))
		//{
		//	cout << "Renaming" << endl;
		//	rename(p, "C:\\Test2");
		//	//cout << "Deleting" << endl;
		//	//boost::filesystem::remove("C:\\Test2");
		//}
	}
	catch (filesystem_error &e)
	{
		std::cerr << e.what() << '\n';
	}


	return 0;
}