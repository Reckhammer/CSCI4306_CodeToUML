#include "FileFormatter.h"

#include <filesystem>
#include <vector>

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
	{
		// Only read if its a java file
		if ( entry.path().extension().string().compare(".java") == 0 )
			readFile(entry.path().string(), writeFile);
	}
}

void readFile(std::string filePath, FILE* writeFile)
{
	FILE* srcFile = fopen(filePath.c_str(), "r"); // open the file to read

	char* line;
	size_t len = 0;

	// while there are lines to read
	//	read the line
	while (fgets(line, len, srcFile) != NULL)
	{
		readLine(line, writeFile);
	}

	fputs("}\n\n", writeFile); // put the ending for the class

	fclose(srcFile);
}

void readLine(std::string line, FILE* writeFile)
{
	line = trim(line); // trim whitespace off the ends

	std::vector<std::string> tokens; // container for all tokens in the line
	tokenize(line, ' ', tokens);

	std::string out = ""; // will append all usuable tokens of the line for file writing

	bool hasVisMod = false;
	bool hasClass = false;
	bool hasDataType = false;

	for (int ind = 0; ind < tokens.size(); ind++)
	{
		if (tokens[ind].find("//") != std::string::npos) // if its a comment, stop reading the line
			break;

		if (isKeyword(tokens[ind])) // if its a non function or variable keyword, skip
			return;

		if (isVisMod(tokens[ind]))
		{
			hasVisMod = true;
			addVisMod(out, tokens[ind]);
		}
		else if (isClassType(tokens[ind]))
		{
			hasClass = true;
			addClassType(out, tokens);
			break;
		}
		else if (isDataType(tokens[ind]))
		{
			hasDataType = true;
			addDataType(out, tokens[ind]);
		}
		else if (tokens[ind].find('(') != std::string::npos) // its a function
		{
			addFunction(out, tokens, ind);
			break;
		}
		else // its a variable
		{
			out.append(tokens[ind]);
		}
	}

	if (out.size() >= 0)
		fputs((out+"\n").c_str(), writeFile);
}

std::string* FileFormatter::getTokens(std::string line, std::string delim)
{
	return 0;
}

bool isKeyword(std::string word)
{
	static const std::string keywords[] = { "assert", "break", "case", "catch", "continue", "default", "do", "else",
										"final", "finally", "for", "if", "import", "instanceof", "native", "new",
										"null", "package", "return", "short", "static", "strictfp", "super", "switch", "synchronized",
										"this", "throw", "throws", "transient", "try", "void", "volatile", "while" };

	// Cut off anything after a parathesis
	if (word.find('(') != std::string::npos)
		word = word.substr(0, word.find('('));

	for (int ind = 0; ind < 36; ind++)
	{
		if (word.compare(keywords[ind]) == 0)
			return true;
	}

	return false;
}

std::string trim(std::string str)
{
	const char* typeOfWhitespaces = " \t\n\r\f\v";
	str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
	str.erase(0, str.find_first_not_of(typeOfWhitespaces));
	return str;
}

void tokenize(const std::string& str, const char delim, std::vector<std::string>& out)
{
	std::string::size_type beg = 0;
	for (auto end = 0; (end = str.find(delim, end)) != std::string::npos; ++end)
	{
		out.push_back(str.substr(beg, end - beg));
		beg = end + 1;
	}

	out.push_back(str.substr(beg));
}

// boolean, byte, char, double, float, long, short
// class, interface
// extends, implements
// private, protected, public, static, void
