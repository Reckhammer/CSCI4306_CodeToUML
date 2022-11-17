//Author: Adrian Cardona

#include <iostream> //input/output and file read/write
#include <string> //the string itself
#include <vector> //creating a list of files
#include <fstream> //for reading and writing to files

inline void keep_window_open() {char ch; std::cin>>ch;}

std::string promptForPath();
bool isValidPath(std::string user_input);
int selectionType();
void individualSelection(std::vector<std::string> file_List);
std::vector<std::string> removeFiles(std::vector<std::string> file_List);
void showListofFiles(std::vector<std::string> file_List);
int inputValidity(int input_range);
void recursiveSelection(std::vector<std::string> file_List);

int main() {
	std::string file_path;
	int file_Select_Type = 0;
	std::vector<std::string> file_List;

	std::cout << "Welcome to the Code-to-UML Program." << std::endl << std::endl;

	while(file_Select_Type == 0) {
		file_Select_Type = selectionType();

		if(file_Select_Type == 0) {
			std::cout << "That is not a valid selection." << std::endl;
		}
	}

	switch(file_Select_Type) {
		case 1:
			std::cout << "Perform individual file selection." << std::endl;
			individualSelection(file_List);
			break;
		case 2:
			std::cout << "Perform recursive selection." << std::endl;
			break;
		case 3:
			std::cout << "Now exiting program." << std::endl;
			break;
		default:
			std::cout << "Incorrect submission somehow got through." << std::endl;
			break;
	}

	std::cout << "All done, press any button to exit. (Work in progress)" << std::endl;

	keep_window_open();
}

std::string promptForPath() {
	bool run = true;
	std::string file_path;

	while(run) {
		std::cout << "Please enter the file/folder location of the code you'd like to produce a UML diagram for: " << std::endl
			<< "Make sure the file is located in the 'Java Code' folder!" << std::endl;
		std::getline(std::cin >> std::ws, file_path);

		std::fstream fs;

		fs.open(std::string("Java Code\\" + file_path));

		if(fs.fail()) {
			std::cout << file_path << " doesn't exist or is corrupted." << std::endl;
		}

		if(isValidPath(file_path) == true) {
			run = false;
		}
	}

	return file_path;
}

bool isValidPath(std::string user_input) {
	bool validPath = false;
	std::string correctPath = "";
	std::string temp = user_input;
	bool run = true;

	while(run) {
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
	}

	return validPath;
}

int selectionType() {
	int select_Option = 0;
	int result = 0;

	std::cout << "What type of file selection would you like to do?" << std::endl
		<< "1. Individual selection" << std::endl
		<< "2. Recursive file selection" << std::endl
		<< "3. Cancel" << std::endl;

	select_Option = inputValidity(3);

	if(select_Option >= 1 && select_Option <= 3) {
		result = select_Option;
	}

	return result;
}

void individualSelection(std::vector<std::string> file_List) {
	bool run = true;
	int select_Option = 0;

	std::vector<std::string> temp_List;

	temp_List.push_back(promptForPath());

	while(run) {
		std::cout << "Here are the files that will be used to create the UML diagram:" << std::endl;

		showListofFiles(temp_List);

		std::cout << "Would you like to:" << std::endl
			<< "1. Add a new file" << std::endl
			<< "2. Remove unwanted files" << std::endl
			<< "3. Confirm selection" << std::endl
			<< "4. Cancel operation" << std::endl;

		select_Option = inputValidity(4);

		std::cout << "Performing option " << select_Option << std::endl;

		switch(select_Option) {
			case 1:
				temp_List.push_back(promptForPath());
				break;
			case 2:
				temp_List = removeFiles(temp_List);
				break;
			case 3:
				//Here is where you make the formatted text file to use with plantUML
				std::cout << "Here is where you go forward with the selected files."
					<< std::endl;
				run = false;
				break;
			case 4:
				std::cout << "Here is where you cancel the operation."
					<< std::endl;
				run = false;
				break;
			default:
				std::cout << "Invalid option got through.";
				break;
		}
	}
}

std::vector<std::string> removeFiles(std::vector<std::string> file_List) {
	int user_input;
	std::vector<std::string> temp_List;

	temp_List = file_List;

	showListofFiles(temp_List);

	std::cout << "What file would you like to remove? (Ex: \"1\" or \"10\")"
		<< std::endl << "Warning! Does not remove multiple files at once!"
		<< std::endl;

	user_input = inputValidity(temp_List.size());

	temp_List.erase(temp_List.begin() + (user_input - 1));

	std::cout << "Removing file." << std::endl;

	return temp_List;
}

void showListofFiles(std::vector<std::string> file_List) {
	for(int i = 0; i < int(file_List.size()); i++) {
		std::cout << (i + 1) << ". " << file_List[i] << std::endl;
	}
}

int inputValidity(int input_range) {
	bool run = true;
	int input = 0;

	while(run) {
		std::cin >> input;

		if(input >= 1 && input <= input_range) {
			run = false;
		}
		else {
			std::cout << "That is not an acceptable option. Please try again."
				<< std::endl;
		}
	}

	return input;
}

void recursiveSelection(std::vector<std::string> file_List) {
	std::cout << "Perform recursive selection from here" << std::endl;
}