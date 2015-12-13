#pragma once
#include <stdio.h>
class FileWriter
{
public:
	FileWriter(const char* pFileName);
	~FileWriter();

	void writeString(const char* fmt, ...);

private:
	FILE* mFile;
};
