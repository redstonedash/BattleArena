#include <iostream>
#include <fstream>
#include <string>
#pragma warning(push)
#pragma warning( disable : 4996)

std::fstream file;

void saveRecord(char* fullPath, char* partPath) {
	std::string ID;
	std::string data;

	std::cout << "what is the name of your entry? WARNING: entering an existing entry will update it's contents\n";
	std::getline(std::cin, ID);
	std::cout << "what is your entry?\n";
	std::getline(std::cin, data);
	std::fstream tempFile;
	char* modifiedPath = std::strcat(std::strcat(partPath, ID.c_str()), ".bin");
	std::cout << modifiedPath << "\n";
	if (!remove(modifiedPath)){
		file << ID << "\n";
	}
	tempFile.open(modifiedPath, std::ios::app | std::ios::binary);
	tempFile << data;
	file.flush();
	file.close();
	tempFile.flush();
	tempFile.close();

}

void displayRecord(char* fullPath, char* partPath) {
	std::string ID;
	std::cout << "what is the name of your entry?\n";
	std::getline(std::cin, ID);
	char data[] = {0};
	file.seekg(0, std::ios::beg);
	bool isFound = false;
	bool matchingSoFar = true;
	int index = 0;
	while (true){ //confirm that this bin file exist in the data base
		file.read((char*)&data, 1);
		if (file.eof()) {
			break;
		}
		if (matchingSoFar) {
			if (index == ID.length() && data[0] == '\n') {
				isFound = true;
				break;
			}
			if (ID.length()>index && ID.at(index)!=data[0]) {
				matchingSoFar = false;
				index = 0;
			} else if(ID.length()>index) {
				index++;
			} else {
				index = 0;
				matchingSoFar = false;
			}
		}
		if (data[0] == '\n') {
			index = 0;
			matchingSoFar = true;
		}
	}
	if (isFound) {
		std::fstream tempFile;
		char* modifiedPath = std::strcat(std::strcat(partPath, ID.c_str()), ".bin");
		tempFile.open(modifiedPath, std::ios::in | std::ios::binary);
		tempFile.seekg(0, std::ios::beg);
		while (true) {
			tempFile.read((char*)&data, 1);
			if (!tempFile.eof()) {
				std::cout << data[0];
			} else {
				break;
			}
		}
		return;
	}
	else {
		std::cout << "Entry not found!\n";
	}

	return;
}


void help() {
	std::cout <<
		"Hello! Welcome to Data Manager"
		"the second parameter should be what you want to do"
		"the first parameter you give us should be the name of the data base"
		"you can either (s)ave a new record, (d)isplay a record"
		<< "\n";
}

int main(int argc, char* argv[]) {
	if (argc <= 0 || argv[1][0] != 's' || argv[1][0] != 'd') {
		help();
		return 0;
	}
	for (int i = std::strlen(argv[0]); i > 0; i--) {
		if (argv[0][i] != '\\') {
			argv[0][i] = '\0';
		} else { break; }
	}
	char * newArray = new char[std::strlen(argv[0]) + std::strlen(argv[2]) + 1];
	int i = 0;
	for (; i < std::strlen(newArray) + 1; i++) {
		if (i <= std::strlen(argv[0])&&!(argv[0][i]=='\0')) {
			newArray[i] = argv[0][i];
		} else {
			break;
		}
	}
	for (int j = 0; i < std::strlen(newArray) + 1; i++) {
		newArray[i] = argv[2][j];
		j++;
	}
	std::fstream temp;
	temp.open(newArray, std::ios::app | std::ios::binary);
	temp.close();
	switch (argv[1][0]) {
	case 's':
		file.open(newArray, std::ios::app | std::ios::binary);
		saveRecord(newArray, argv[0]);
		return 0;
	case 'd':
		file.open(newArray, std::ios::in | std::ios::binary);
		displayRecord(newArray, argv[0]);
		return 0;
	default:
		help();
		return 0;
	
	}
}
#pragma warning(pop)
