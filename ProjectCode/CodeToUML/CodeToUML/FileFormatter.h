#pragma once
#ifndef FILEFORMATTER_H
#define FILEFORMATTER_H

#include <string>
#include <vector>
#include <stdio.h>

class FileFormatter
{
private:
	std::string srcFilePath;
	std::string destFilePath;

public:
	FileFormatter(std::string, std::string);
	FILE* createFormattedFile(std::string);
	FILE* createFormattedFile(std::vector<std::string>);

private:
	void readProject(std::string, FILE*);
	void readFile(std::string, FILE*);
	std::string getFormattedLine(std::string);
	bool isFileHeader(std::string);
	std::string getFileHeader(std::string);
	bool hasRelation(std::string);
	std::string getRelation(std::string);
	bool hasVisMod(std::string);
	std::string getVisMod(std::string);
	std::string trim(std::string);
	void tokenize(const std::string&, const char, std::vector<std::string>&);
};

#endif