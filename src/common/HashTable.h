#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include "String.h"

typedef struct hashnode
{
	lfStr key;
	void* value;
	hashnode* next;
}hashnode;

class HashTable
{
public:
	HashTable( int newtablesize = 256 );
	~HashTable();

	void* Get(lfStr key);
	void Put(lfStr key, void* value);

private:
	hashnode* HashStr(lfStr key);

	void Resize(int size);
	void Free();
	hashnode* GetFreePos();
	hashnode* NewKey(lfStr key);
protected:

private:
	hashnode* node;
	int tablesize;
};


#endif


