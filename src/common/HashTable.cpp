#include "HashTable.h"

static int Hash( const char *string ) {
	int i, hash = 0;
	for ( i = 0; *string != '\0'; i++ ) {
		hash += ( *string++ ) * ( i + 119 );
	}
	return hash;
}

HashTable::HashTable( int newtablesize /*= 256 */ ):node(NULL), tablesize(0)
{
	Resize(newtablesize);
}

HashTable::~HashTable()
{
	Free();
}

void* HashTable::Get( lfStr key )
{
	hashnode* n = HashStr(key);
	do { 
		if ( n->key.Length()!=0 && n->key == key)
			return n->value;  /* that's it */
		else n = n->next;
	} while (n);
	return NULL;
}

void HashTable::Put( lfStr key, void* value )
{
	hashnode* node = HashStr(key);
	if (node->value == NULL)
	{
		node->value = value;
	}
	else
	{
		hashnode* node = NewKey(key);
		node->value = value;
	}
}

hashnode* HashTable::HashStr( lfStr key )
{
	int pos = Hash(key.c_str()) & (tablesize-1);
	return &node[pos];
}

void HashTable::Resize(int size)
{
	Free();
	tablesize = size;
	node = new hashnode[size];
	memset( node, 0, tablesize * sizeof( node[0] ) );
}

void HashTable::Free( void ) {
	if ( node != NULL ) {
		delete[] node;
		node = NULL;
	}
}

hashnode* HashTable::GetFreePos() 
{
	for (int i=0; i<tablesize; i++)
		if (node[i].value == NULL)
			return &node[i];

	return NULL;
}

hashnode* HashTable::NewKey( lfStr key )
{
	hashnode* node = HashStr(key);
	hashnode* freeNode = GetFreePos();
	if (freeNode == NULL)
	{
		//TODO:Resize
	}
	else
	{
		while (node->next != NULL)
			node = node->next;
		node->next = freeNode;
	}
	return freeNode;
}
