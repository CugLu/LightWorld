#ifndef __MEM_H__
#define __MEM_H__

typedef struct {
	int		num;
	int		minSize;
	int		maxSize;
	int		totalSize;
} memoryStats_t;

void* mem_alloc( const int size );

void mem_free( void *ptr );

void* mem_cleared_alloc( const int size );

#endif /* !__HEAP_H__ */
