#ifndef __FILE_H__
#define __FILE_H__

#include "common/String.h"
#include "common/Vec3.h"
#include "common/Quat.h"

// mode parm for Seek
typedef enum {
	FS_SEEK_SET,
	FS_SEEK_CUR,
	FS_SEEK_END,
} fsOrigin_t;

class lfBuffer
{
public:
	 lfStr ReadString();
	 int ReadInt();
	 float ReadFloat();
	 int ReadChar();
	 Vec3 ReadVec3();
	 Quat ReadQuat();

	 unsigned int ReadUnsignedInt();
	 unsigned short ReadUnsignedShort();
	 unsigned char ReadUnsignedChar();

	 virtual int Length();
	 virtual int Read( void *buffer, int len );
	 virtual int Seek( long offset, fsOrigin_t origin );
};

class lfFile : public lfBuffer
{
public:
	lfFile()  {}
	~lfFile();

	bool Open( const char* filepath, const char* mode = "rb");

	int Tell();
public:
	virtual int Read( void *buffer, int len );
	virtual int Length();
	virtual int Seek( long offset, fsOrigin_t origin );
	virtual int Skip( int len );

	void WriteString(const char* fmt, ...);
	//lfFile& operator>>(unsigned char& value)
	//{
	//	value = ReadUnsignedChar();
	//	return *this;
	//}

private:
	FILE* fp;
	int current;
	int fileSize;
};

char* F_ReadFileData( const char* filename );
#endif

