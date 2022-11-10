//Author: Adrian Cardona

#include <iostream> //input/output
#include <string> //the string itself
//#include <filesystem> //To take in file path input

#include <FileFormatter.h>

inline void keep_window_open() {char ch; std::cin>>ch;}

std::string promptForPath(std::string user_input);
bool isValidPath(std::string user_input);

int main() {
	std::string file_path;

	std::cout << "Welcome to the Code-to-UML Program." << std::endl;
	promptForPath(file_path);

	std::cout << "All done, press any button to exit. (Work in progress)" << std::endl;

	keep_window_open();
}

std::string promptForPath(std::string user_input) {
	bool run = true;

	while(run) {
		std::cout << "Please enter the file location of the code you'd like to produce a UML diagram for: " << std::endl;
		std::cin >> user_input;

		if(isValidPath(user_input) == true) {
			run = false;
		}
	}

	return user_input;
}

bool isValidPath(std::string user_input) {
	std::string correctPath = "";
	std::string temp = user_input;
	bool run = true;
	//std::filesystem::path path(temp);

	while(run) {
		bool validPath = false;
		std::cout << "Is '" << user_input << "' the correct path? (y/n)" << std::endl;
		std::cin >> correctPath;

		if(correctPath == "y") {
			run = false;
			validPath = true;
		}
		else if(correctPath == "n") {
			run = false;
			validPath = false;
		}
		else {
			std::cout << "That is not the appropriate response." << std::endl;
		}
		
		return validPath;
	}
}

void testFileFormattter()
{
	std::string testPath = "C:\\Users\\JEDIJ\\Documents\\C\\CSCI4316_CodeToUML\\ProjectCode\\CodeToUML";
	FileFormatter formatter(testPath);

	formatter.createFormattedFile();
}
