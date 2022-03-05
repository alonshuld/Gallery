#include <iostream>
#include <string>
#include <ctime>
#include "MemoryAccess.h"
#include "AlbumManager.h"


int getCommandNumberFromUser()
{
	std::string message("\nPlease enter any command(use number): ");
	std::string numericStr("0123456789");
	
	std::cout << message << std::endl;
	std::string input;
	std::getline(std::cin, input);
	
	while (std::cin.fail() || std::cin.eof() || input.find_first_not_of(numericStr) != std::string::npos) {

		std::cout << "Please enter a number only!" << std::endl;

		if (input.find_first_not_of(numericStr) == std::string::npos) {
			std::cin.clear();
		}

		std::cout << std::endl << message << std::endl;
		std::getline(std::cin, input);
	}
	
	return std::atoi(input.c_str());
}

int main(void)
{
	// initialization data access
	MemoryAccess dataAccess;

	// initialize album manager
	AlbumManager albumManager(dataAccess);

	startScreenPrinter();
	
	do {
		int commandNumber = getCommandNumberFromUser();
		
		try	{
			albumManager.executeCommand(static_cast<CommandType>(commandNumber));
		} catch (std::exception& e) {	
			std::cout << e.what() << std::endl;
		}
	}
	while (true);
}


void startScreenPrinter()
/*
* This function will print the start message with the stats.
* Input: None.
* Output: None.
*/
{
	//time var
	time_t tmNow;

	//assigning it to current time
	tmNow = time(NULL);

	//putting the time into a struct
	struct tm t = *localtime(&tmNow);

	std::cout << "Welcome to AlonSD's Gallery!" << std::endl;
	std::cout << " " << t.tm_mday << "/" << t.tm_mon + 1 << "/" << t.tm_year + 1900 << "\t    " << t.tm_hour << ":" << t.tm_min << std::endl;
	std::cout << "============================" << std::endl;
	std::cout << "Type " << HELP << " to a list of all supported commands" << std::endl;
}