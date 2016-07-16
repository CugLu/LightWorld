#ifndef __FLIP_LIST_H__
#define __FLIP_LIST_H__

typedef struct list_node
{
	list_node* prev;
	list_node* next;
	void* value;
}list_node;

typedef struct list {
	list_node *head;
	list_node *tail;
	void (*free)(void *ptr);
	unsigned long len;
} list;

list* create_list()
{
	list l = mem_alloc()

}

#endif
