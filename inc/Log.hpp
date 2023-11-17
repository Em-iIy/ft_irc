#pragma once

#include <string>
#include <fstream>

class Log {
private:
	std::ofstream	_file;
	bool			_init;

public:
	Log(void);
	~Log(void);
	void init(const std::string &filename);

	void	logToFile(const std::string &src, const std::string &str);
};
