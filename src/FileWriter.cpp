#include "FileWriter.h"
#include <stdarg.h>
#include <string.h>
FileWriter::FileWriter(const char* pFileName) :mFile(NULL)
{
	mFile = fopen(pFileName, "w");
	if (!mFile)
		printf("fileOpen %s error", pFileName);
}

FileWriter::~FileWriter()
{
	if (mFile)
	{
		fclose(mFile);
		mFile = NULL;
	}
}

void FileWriter::writeString(const char* fmt, ...)
{
	char szBuf[255];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(szBuf, 255, fmt, ap);
	va_end(ap);
	fwrite(szBuf, strlen(szBuf), 1, mFile);
}