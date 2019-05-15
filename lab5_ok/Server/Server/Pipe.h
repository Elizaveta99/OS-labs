#pragma once
//#include<string>

struct Pipe
{
	int amount;
	HANDLE handleReadPipe;
	HANDLE *handlesWritePipe;
	std::string* names;
};