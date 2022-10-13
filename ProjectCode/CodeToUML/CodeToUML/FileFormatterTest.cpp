// CodeToUML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "FileFormatter.h"

int main()
{
    std::string testPath = "C:\\Users\\JEDIJ\\Documents\\C\\CSCI4316_CodeToUML\\ProjectCode\\CodeToUML";
    FileFormatter formatter(testPath);

    formatter.createFormattedFile();
}

