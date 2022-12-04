//Author: Adrian Cardona

#include <iostream> //input/output and file read/write
#include <string> //the string itself
#include <vector> //creating a list of files
#include <fstream> //for reading and writing to files

#include "FileFormatter.h"

inline void keep_window_open() {char ch; std::cin>>ch;}

std::string promptForPath();
int selectionType();
void individualSelection(std::vector<std::string> file_List);
std::vector<std::string> removeFiles(std::vector<std::string> file_List);
void showListofFiles(std::vector<std::string> file_List);
int inputValidity(int input_range);

int main() {
	std::string file_path;
	int file_Select_Type = 0;
	std::vector<std::string> file_List;
	bool run = true;

	std::cout << "Welcome to the Code-to-UML Program." << std::endl << std::endl;

	while(run) {
		file_Select_Type = selectionType();

		switch(file_Select_Type) {
			case 1:
				std::cout << std::endl << "Perform individual file selection." << std::endl;
				individualSelection(file_List);
				break;
			case 2:
				std::cout << std::endl << "Now exiting program." << std::endl;
				run = false;
				break;
			default:
				std::cout << std::endl << "Incorrect submission somehow got through." << std::endl;
				break;
		}
	}

	std::cout << std::endl << "All done, press any button to exit." << std::endl;

	//keep_window_open();
}

std::string promptForPath() {
	bool run = true;
	std::string file_path;

	const std::string fileDeposit = ".\\JavaCode\\";

	while(run) {
		std::cout << "Please enter the file name of the code you'd like to produce a UML diagram for: " << std::endl
			<< "Make sure the file is located in the 'JavaCode' folder!" << std::endl
			<< "Needs to be a java file!" << std::endl
			<< "Enter 'x' to cancel (Case sensitive!)" << std::endl
			<< "File name: ";
		std::getline(std::cin >> std::ws, file_path);

		if(file_path == "x") {
			std::cout << std::endl << "Cancelling." << std::endl << std::endl;
			run = false;
		}
		else {
			std::fstream fs;

			fs.open(std::string(fileDeposit + file_path));

			std::string temp_string = fileDeposit + file_path;

			if(fs.fail()) {
				std::cout << std::endl << file_path << " doesn't exist or is corrupted." << std::endl << std::endl;
			}
			else {
				if(temp_string.substr(temp_string.find_last_of(".") + 1) == "java") {
				    run = false;
				} 
				else {
				    std::cout << std::endl << "File type not supported." << std::endl << std::endl;
				}
			}
			fs.close();
		}
	}
	return fileDeposit + file_path;
}

int selectionType() {
	int select_Option = 0;
	int result = 0;

	std::cout << "What type of file selection would you like to do?" << std::endl
		<< "1. Individual selection" << std::endl
		<< "2. Exit program" << std::endl
		<< ">> ";

	select_Option = inputValidity(3);

	if(select_Option >= 1 && select_Option <= 3) {
		result = select_Option;
	}

	return result;
}

void individualSelection(std::vector<std::string> file_List) {
	bool run = true;
	int select_Option = 0;
	std::string file_name;

	FileFormatter formatter("", "");

	std::vector<std::string> temp_List;

	file_name = promptForPath();

	if(file_name != ".\\JavaCode\\x") {
		temp_List.push_back(file_name);
	}

	while(run) {
		int duplicate = 0;

		std::cout << std::endl << "Here are the files that will be used to create the UML diagram:" << std::endl;

		showListofFiles(temp_List);

		std::cout << std::endl << "Would you like to:" << std::endl
			<< "1. Add a new file" << std::endl
			<< "2. Remove unwanted files" << std::endl
			<< "3. Confirm selection" << std::endl
			<< "4. Cancel operation" << std::endl
			<< ">> ";

		select_Option = inputValidity(4);

		std::cout << std::endl << "Performing option " << select_Option << std::endl;

		switch(select_Option) {
			case 1:
				file_name = promptForPath();

				if(file_name != ".\\JavaCode\\x") {
					temp_List.push_back(file_name);
				}

				for (int i = 0; i < int(temp_List.size()); i++) {
				if (file_name == temp_List[i]) {
					duplicate++;
					}
				}

				if (duplicate >= 2) {
					std::cout << "Operation failed. File already exists in selection." << std::endl << std::endl;
					temp_List.pop_back();
				}

				break;
			case 2:
				if(temp_List.size() == 0) {
					std::cout << "There is nothing to remove!" << std::endl
					<< std::endl;
				}
				else {
					temp_List = removeFiles(temp_List);
				}
				break;
			case 3:
				//Here is where you make the formatted text file to use with plantUML
				std::cout << std::endl << "Here is where you go forward with the selected files." << std::endl;
				if(temp_List.size() == 0) {
					std::cout << std::endl << "There are no files to use." << std::endl;
				}

				formatter.createFormattedFile(temp_List);

				run = false;
				break;
			case 4:
				std::cout << "Operation cancelled."
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
