#include <locale>
#include <codecvt>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <regex>

using namespace std;
using namespace std::filesystem;

wifstream openWif(string* fileName)
{

	wifstream stream(*fileName);
	stream.imbue(std::locale("Korean"));
	return stream;
}

wofstream openWof(string* fileName)
{
	wofstream stream(*fileName);
	stream.imbue(std::locale("en_US.UTF-8"));
	return stream;
}

void iterate_directory(string*);
int main(int argc, char* argv[])
{
	std::wcout.imbue(std::locale(""));

	string rootPath = "./";
	if(argc == 2)
	{
		rootPath = argv[1];
	}
	
	iterate_directory(&rootPath);

	return 0;
}

void iterate_directory(string* root_directory)
{
	regex re(".*.[c|h](p{2})?");
	regex re2(".*SFML.*");
	smatch m;
	for (const auto& p : directory_iterator(*root_directory))
	{
		string currPathStr = p.path().string();
		//cout << p.path().string() << endl;
		if(p.is_directory())
		{
			string newRoot = p.path().string();
			iterate_directory(&newRoot);
		}

		if (regex_match(currPathStr, m, re) && !regex_match(currPathStr, m, re2))
		{
			cout << currPathStr << endl << endl;
			wifstream currFile = openWif(&currPathStr );
			wstring buff;

			string newName = currPathStr + ".cvt";

			wofstream currWriteFile = openWof(&newName);

			while (getline(currFile, buff))
			{
				//wcout << buff << endl;
				currWriteFile << buff << endl;
			}
			currFile.close();
			currWriteFile.close();

			remove(currPathStr);
			rename(newName, currPathStr);

		}
	}
}