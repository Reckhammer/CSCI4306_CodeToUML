#pragma once
#ifndef FILEFORMATTER_H
#define FILEFORMATTER_H

#include <string>
#include <stdio.h>

class FileFormatter
{
private:
	std::string srcFilePath;

public:
	FileFormatter(std::string);
	FILE* createFormattedFile();

private:
	void readProject(std::string);
	void readFile(std::string);
	std::string* getTokens(std::string, std::string);
	bool isKeyword(std::string);
};

#endif