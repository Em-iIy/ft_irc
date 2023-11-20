#include "Log.hpp"
#include "utils.hpp"
#include <exception>
#include <iostream>
#include <ctime>

Log::Log(void) 
{
	this->_init = false;
}

void	Log::init(const std::string &filename)
{
	std::string	fname = filename + ".log";
	// Open the file
	this->_file.open(fname);
	if (!this->_file.is_open())
	{
		std::string	error = "Error: Logfile " + std::string(fname) + " could not be opened";
		throw std::runtime_error(error);
	}
	this->_init = true;
}

Log::~Log(void)
{
	// Close the file
	this->_file.close();
}

void	Log::logToFile(const std::string &src, const std::string &str)
{
	time_t		time = std::time(NULL);
	std::string	strTime = std::string(ctime(&time));
	rmCRLF(strTime);
	std::string msg = "[" + strTime + " - " + src + "]\t" + str;

	rmCRLF(msg);
	std::cout << msg << std::endl;
	if (!this->_init)
		return ;
	this->_file << msg << std::endl;
}
