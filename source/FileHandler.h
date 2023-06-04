#pragma once
#include <string>
#include <fstream>
class FileHandler
{
private:
	std::fstream _stream;
	std::string _text;
	std::string _path;
	char* linebuffer = new char[256];
public:
	void Open(std::string);
	std::string ReadAllText(std::string);
	void Close();
	bool is_open();
};

