#ifndef __LINKLIST_H__
#define __LINKLIST_H__

#include "precompiled.h"

template< class type >
class LinkList
{
public:
	LinkList();
	~LinkList();

	bool	 IsEmpty( void ) const;
	int		 Num( void ) const;
	void	 Clear( void );

	void	 InsertBefore( LinkList &node );
	void	 InsertAfter( LinkList &node );
	void	 AddToEnd( LinkList &node );
	void	 AddToFront( LinkList &node );

	void	 Remove( void );

	type*	 Next( void ) const;
	type*	 Prev( void ) const;

	type*	 Owner( void ) const;
	void	 SetOwner( type *object );

	LinkList*	 ListHead( void ) const;
	LinkList*	 NextNode( void ) const;
	LinkList*	 PrevNode( void ) const;

private:
	LinkList*	 head;
	LinkList*	 next;
	LinkList*	 prev;
	type*	 owner;
};


template< class type >
LinkList<type>::LinkList() {
	head = this;
	next = this;
	prev = this;
	owner = 0;
}

template< class type >
LinkList<type>::~LinkList() {
	Clear();
}


template< class type >
void LinkList<type>::Remove( void ) {
	prev->next = next;
	next->prev = prev;

	next = this;
	prev = this;
	head = this;
}

template< class type >
void LinkList<type>::Clear( void ) {
	if ( head == this ) {
		while( next != this ) {
			next->Remove();
		}
	} else {
		Remove();
	}
}

template< class type >
bool LinkList<type>::IsEmpty() const {
	return head->next == head;
}

template< class type >
int LinkList<type>::Num( void ) const {
	LinkList<type>	*node;
	int				num;

	num = 0;
	for( node = head->next; node != head; node = node->next ) {
		num++;
	}

	return num;
}

template< class type >
void LinkList<type>::InsertAfter( LinkList &node ) {
	Remove();

	prev = &node;
	next = node.next;
	node.next = this;
	next->prev = this;
	head = node.head;
}

template< class type >
void LinkList<type>::InsertBefore( LinkList &node ) {
	Remove();

	prev = node.prev;
	next = &node;
	node.prev = this;
	prev->next = this;
	if (node.head != node){
		head = node.head;
	}
}

template< class type >
void LinkList<type>::AddToEnd( LinkList &node ) {
	InsertBefore( *node.head );
}

template< class type >
void LinkList<type>::AddToFront( LinkList &node ) {
	InsertAfter( *node.head );
}

template< class type >
LinkList<type> *LinkList<type>::ListHead( void ) const {
	return head;
}

template< class type >
type *LinkList<type>::Next( void ) const {
	if ( !next || ( next == head ) ) {
		return NULL;
	}
	return next->owner;
}

template< class type >
type *LinkList<type>::Prev( void ) const {
	if ( !prev || ( prev == head ) ) {
		return NULL;
	}
	return prev->owner;
}

template< class type >
LinkList<type> *LinkList<type>::NextNode( void ) const {
	if ( next == head ) {
		return NULL;
	}
	return next;
}

template< class type >
LinkList<type> *LinkList<type>::PrevNode( void ) const {
	if ( prev == head ) {
		return NULL;
	}
	return prev;
}

template< class type >
type *LinkList<type>::Owner( void ) const {
	return owner;
}

template< class type >
void LinkList<type>::SetOwner( type *object ) {
	owner = object;
}

#endif /* !__LINKLIST_H__ */
