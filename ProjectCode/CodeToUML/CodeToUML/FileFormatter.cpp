#include "FileFormatter.h"

// public
FileFormatter::FileFormatter(std::string filePath)
{
	srcFilePath = filePath;
}

FILE* FileFormatter::createFormattedFile()
{
	// get the folder's name to use for text file's name
	size_t slashPos = srcFilePath.rfind('/');
	std::string fileName = srcFilePath.substr(slashPos + 1) +".txt";

	FILE* umlFile;
	fopen_s(&umlFile, fileName.c_str(), "w"); // create a new file to write in
	fputs("@startuml\n", umlFile);



	fputs("@enduml\n", umlFile);
	fclose(umlFile);

	return umlFile;
}

// private
void readProject(std::string projectPath)
{
	// TODO: Implement
}

void readFile(std::string filePath)
{

}

std::string* FileFormatter::getTokens(std::string line, std::string delim)
{
	return 0;
}

bool isKeyword(std::string word)
{
	return false;
}

