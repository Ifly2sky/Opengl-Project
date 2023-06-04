#include "FileHandler.h"
#include <fstream>
#include <iostream>
#include <string>

void FileHandler::Open(std::string path)
{
		_stream.open(path, std::ios_base::in);
}

std::string FileHandler::ReadAllText(std::string path)
{
	Open(path);
	if (this->is_open())
	{
		_text.clear();
		while (_stream.getline(linebuffer, 255))
		{
			_text += linebuffer;
			_text += "\n";
		}
	}
	else {
		//tee joku error logi classi
	}
	Close();
	return _text;
}

void FileHandler::Close()
{
	_stream.close();
}

bool FileHandler::is_open()
{
	return _stream.is_open();
}