#include <stdlib.h>
#include <string.h>

void *mem_alloc( const int size ) {
	if ( !size ) {
		return NULL;
	}

	return malloc( size );
}

void mem_free( void *ptr ) {
	if ( !ptr ) {
		return;
	}

	free( ptr );
}

void *mem_cleared_alloc( const int size ) {
	void *mem = mem_alloc( size );
	memset(mem, 0, size);
	return mem;
}
