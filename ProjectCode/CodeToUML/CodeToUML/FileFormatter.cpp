#include "FileFormatter.h"

#include <filesystem>

namespace fs = std::filesystem; // alias for namespace because I'm lazy to type all that

// public
FileFormatter::FileFormatter(std::string src, std::string dest)
{
	srcFilePath = src;
	destFilePath = dest;
}

FILE* FileFormatter::createFormattedFile()
{
	// get the folder's name to use for text file's name
	size_t slashPos = srcFilePath.rfind('/');
	std::string fileName = srcFilePath.substr(slashPos + 1) +".txt";

	FILE* umlFile;
	fopen_s(&umlFile, fileName.c_str(), "w"); // create a new file to write in
	fputs("@startuml\n", umlFile);
	
	// if src file path is a directory
	//		then read the all contents recursively
	// else
	//		its just 1 file so read it as one
	if (fs::is_directory(srcFilePath))
		readProject(srcFilePath, umlFile);
	else
		readFile(srcFilePath, umlFile);

	fputs("@enduml\n", umlFile);
	fclose(umlFile);

	return umlFile;
}

// private
void readProject(std::string projectPath, FILE* writeFile)
{
	for (const auto entry : fs::recursive_directory_iterator(projectPath))
		readFile(entry.path().string(), writeFile);
}

void readFile(std::string filePath, FILE* writeFile)
{

}

std::string* FileFormatter::getTokens(std::string line, std::string delim)
{
	return 0;
}

bool isKeyword(std::string word)
{
	static std::string keywords[] = { "abstract", "assert", "boolean", "break", "byte", "case", "catch", "char", "class", "continue", "default", "do", "double", "else", "enum", "extends",
										"final", "finally", "float", "for", "if", "implements", "import", "instanceof", "int", "interface", "long", "native", "new",
										"null", "package", "private", "protected", "public", "return", "short", "static", "strictfp", "super", "switch", "synchronized",
										"this", "throw", "throws", "transient", "try", "void", "volatile", "while" };
	return false;
}

// boolean, byte, char, double, float, long, short
// class, interface
// extends, implements
// private, protected, public, static, void
