#include "FileFormatter.h"

#include <filesystem>
#include <stack>

namespace fs = std::filesystem; // alias for namespace because I'm lazy to type all that

// public
FileFormatter::FileFormatter(std::string src = "", std::string dest = "")
{
	srcFilePath = src;
	destFilePath = dest;
}

FILE* FileFormatter::createFormattedFile(std::string srcFile)
{
	// get the folder's name to use for text file's name
	size_t slashPos = srcFile.rfind('\\');
	std::string fileName = srcFile.substr(slashPos + 1) +".txt";

	FILE* umlFile;
	fopen_s(&umlFile, fileName.c_str(), "w"); // create a new file to write in
	fputs("@startuml\n", umlFile);
	
	// if src file path is a directory
	//		then read the all contents recursively
	// else
	//		its just 1 file so read it as one
	if (fs::is_directory(srcFile))
		readProject(srcFile, umlFile);
	else
		readFile(srcFile, umlFile);

	fputs("@enduml\n", umlFile);
	fclose(umlFile);

	return umlFile;
}

FILE* FileFormatter::createFormattedFile(std::vector<std::string> fileList)
{
	FILE* umlFile;
	fopen_s(&umlFile, "plantUML.txt", "w"); // create a new file to write in
	fputs("@startuml\n", umlFile);

	for (std::string file : fileList)
	{
		readFile(file, umlFile);
	}

	fputs("@enduml\n", umlFile);
	fclose(umlFile);

	return umlFile;
}

// private
void FileFormatter::readProject(std::string projectPath, FILE* writeFile)
{
	for (const auto entry : fs::recursive_directory_iterator(projectPath))
	{
		// Only read if its a java file
		if ( entry.path().extension().string().compare(".java") == 0 )
			readFile(entry.path().string(), writeFile);
	}
}

void FileFormatter::readFile(std::string filePath, FILE* writeFile)
{
	FILE* srcFile = fopen(filePath.c_str(), "r"); // open the file to read

	bool hasHeader = false;

	// containers for fgets
	char* line;
	size_t len = 0;

	std::stack<char> brackets;

	// Find the file header i.e. class, interface, etc
	// Loop til the file ends or header is found
	while (fgets(line, len, srcFile) != NULL && !hasHeader)
	{
		hasHeader = isFileHeader(trim(line));
	}

	if (hasHeader)
	{
		fputs(getFileHeader(line).c_str(), writeFile);

		// if there is no {, just add the newline to file
		// else add { and newline to file
		if (trim(line).rfind('{') != std::string::npos)
		{
			fputs("\n", writeFile);
			brackets.push('{');
		}
		else
		{
			fputs("{\n", writeFile);
		}
	}
	else
	{
		printf("There is no class header in file: %s\n", filePath);
		return;
	}

	while (fgets(line, len, srcFile) != NULL && brackets.size() > 0)
	{
		std::string trimmedLine = trim(line);

		if (trimmedLine.find('{') != std::string::npos)
		{
			brackets.push('{');

			if (trimmedLine.find('}') != std::string::npos)
			{
				brackets.pop();
			}

			continue;
		}
		if (trimmedLine.find('}') != std::string::npos)
		{
			brackets.pop();
			continue;
		}

		if (brackets.size() == 1)
		{
			std::string formattedLine = getFormattedLine(trimmedLine);
			fputs(formattedLine.c_str(), writeFile);
		}
	}

	fputs("}\n\n", writeFile); // put the ending for the class

	fclose(srcFile);
}

std::string FileFormatter::getFormattedLine(std::string line)
{
	std::string fStr = "";

	if (hasVisMod(line))
	{
		std::string visMod = getVisMod(line);
		fStr.append(visMod);
		fStr.append(" ");

		int firstSpacePos = line.find(' ');

		fStr.append(line.substr(firstSpacePos+1));
	}
	else
	{
		fStr.append("-");
		fStr.append(line);
	}

	fStr.append("\n");
	return fStr;
}

bool FileFormatter::isFileHeader(std::string line)
{
	// split the line into tokens
	// check if any tokens are class header keywords
	// if there is a keyword, return true
	static const std::string headers[] = { "enum", "interface", "class" };

	for (std::string header : headers)
	{
		if (line.find(header) != std::string::npos)
			return true;
	}

	return false;
}

std::string FileFormatter::getFileHeader(std::string line)
{
	std::string fStr = "";

	if (hasVisMod(line))
	{
		line = line.substr(line.find(' ') + 1);
	}

	if (hasRelation(line))
	{
		fStr.append(getRelation(line));
	}
	else
	{
		fStr.append(line);
	}

	return fStr;
}

bool FileFormatter::hasRelation(std::string line)
{
	static const std::string relations[] = { "extends", "implements" };

	for (std::string relation : relations)
	{
		if (line.find(relation) != std::string::npos)
			return true;
	}

	return false;
}

std::string FileFormatter::getRelation(std::string line)
{
	std::string fStr = "";
	size_t relationPos = std::string::npos;
	std::string relationSymbol = "";

	if (line.find("extends") != std::string::npos)
	{
		relationPos = line.find("extends");
		relationSymbol = "--|>";
	}
	else if (line.find("implements") != std::string::npos)
	{
		relationPos = line.find("implements");
		relationSymbol = "-->";
	}

	// The relationship line i.e. class1 --|> class2
	fStr.append(line.substr(line.find(' ') + 1, relationPos));
	fStr.append(" "+ relationSymbol +" ");
	fStr.append(line.substr(line.rfind(' ') + 1)+"\n");

	fStr.append(line.substr(0, relationPos));

	return fStr;
}

bool FileFormatter::hasVisMod(std::string line)
{
	static const std::string visMods[] = { "private", "public", "protected" };

	for (std::string visMod : visMods)
	{
		if (line.find(visMod) != std::string::npos)
			return true;
	}

	return false;
}

std::string FileFormatter::getVisMod(std::string line)
{
	std::string visModWord = line.substr(line.find(' ') + 1);

	if (visModWord.compare("private") == 0)
		return std::string("-");
	else if (visModWord.compare("public") == 0)
		return std::string("+");
	else
		return std::string("#");
}

std::string FileFormatter::trim(std::string str)
{
	const char* typeOfWhitespaces = " \t\n\r\f\v";
	str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
	str.erase(0, str.find_first_not_of(typeOfWhitespaces));
	return str;
}

void FileFormatter::tokenize(const std::string& str, const char delim, std::vector<std::string>& out)
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
