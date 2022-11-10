#pragma once
#ifndef FILEFORMATTER_H
#define FILEFORMATTER_H

#include <string>
#include <stdio.h>

class FileFormatter
{
private:
	std::string srcFilePath;
	std::string destFilePath;

public:
	FileFormatter(std::string, std::string);
	FILE* createFormattedFile();

private:
	void readProject(std::string, FILE*);
	void readFile(std::string, FILE*);
	std::string* getTokens(std::string, std::string);
	bool isKeyword(std::string);
};

#endif