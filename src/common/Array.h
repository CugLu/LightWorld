#ifndef __ARRAY_H__
#define __ARRAY_H__

template <class T>
class Array
{
public:
	Array();

	Array(unsigned int start_count);

	Array(const Array<T>& other);

	~Array();

	void reallocate(unsigned int new_size);

	void push_back(const T& element);

	void clear();

	//! Sets pointer to new array, using this as new workspace.
	//! \param newPointer: Pointer to new array of elements.
	//! \param size: Size of the new array.
	void set_pointer(T* newPointer, unsigned int size);

	//! Sets if the array should delete the memory it used.
	//! \param f: If true, the array frees the allocated memory in its
	//! destructor, otherwise not. The default is true.
	void set_free_when_destroyed(bool f);

	//! Sets the size of the array.
	//! \param usedNow: Amount of elements now used.
	void set_used(unsigned int usedNow);

	//! Assignement operator
	void operator=(const Array<T>& other);

	//! Direct access operator
	T& operator [](unsigned int index);

	//! Direct access operator
	const T& operator [](unsigned int index) const;

	//! Returns a pointer to the array.
	//! \return Pointer to the array.
	T* pointer();

	//! Returns a const pointer to the array.
	//! \return Pointer to the array.
	const T* const_pointer() const;

	//! Returns size of used array.
	//! \return Size of elements in the array.
	unsigned int size() const;

	//! Returns amount memory allocated.
	//! \return Returns amount of memory allocated. The amount of bytes
	//! allocated would  be allocated_size() * sizeof(ElementsUsed);
	unsigned int allocated_size() const;

	//! Returns true if array is empty
	//! \return True if the array is empty, false if not.
	bool empty() const;

	//! Performs a binary search for an element, returns -1 if not found.
	//! The array will be sorted before the binary search if it is not
	//! already sorted.
	//! \param element: Element to search for.
	//! \return Returns position of the searched element if it was found,
	//! otherwise -1 is returned.
	int binary_search(const T& element);

	//! Performs a binary search for an element, returns -1 if not found.
	//! The array will be sorted before the binary search if it is not
	//! already sorted.
	//! \param element: Element to search for.
	//! \param left: First left index
	//! \param right: Last right index.
	//! \return Returns position of the searched element if it was found,
	//! otherwise -1 is returned.
	int binary_search(const T& element, int left, int right);

	//! Erases an element from the array. May be slow, because all elements 
	//! following after the erased element have to be copied.
	//! \param index: Index of element to be erased.
	void erase(unsigned int index);

	//! Erases some elements from the array. may be slow, because all elements 
	//! following after the erased element have to be copied.
	//! \param index: Index of the first element to be erased.
	//! \param count: Amount of elements to be erased.
	void erase(unsigned int index, int count);

	T& end();

	private:
		T* data;
		unsigned int allocated;
		unsigned int used;
		bool free_when_destroyed;
		bool is_sorted;
};

template <class T>
Array<T>::Array() : data(0), used(0), allocated(0),
	free_when_destroyed(true), is_sorted(true)
{

}

template <class T>
Array<T>::Array( unsigned int start_count ) : data(0), used(0), allocated(0),
	free_when_destroyed(true),	is_sorted(true)
{
	reallocate(start_count);
}

template <class T>
Array<T>::Array( const Array<T>& other ) : data(0)
{
	*this = other;
}

template <class T>
Array<T>::~Array()
{
	if (free_when_destroyed)
		delete [] data;
}

template <class T>
void Array<T>::reallocate( unsigned int new_size )
{
	T* old_data = data;

	data = new T[new_size];
	allocated = new_size;

	int end = used < new_size ? used : new_size;
	for (int i=0; i<end; ++i)
		data[i] = old_data[i];

	if (allocated < used)
		used = allocated;

	delete [] old_data;
}

template <class T>
void Array<T>::push_back( const T& element )
{
	if (used + 1 > allocated)
		reallocate(used * 2 +1);

	data[used++] = element;
	is_sorted = false;
}

template <class T>
void Array<T>::clear()
{
	delete [] data;
	data = 0;
	used = 0;
	allocated = 0;
	is_sorted = true;
}

template <class T>
void Array<T>::set_pointer( T* newPointer, unsigned int size )
{
	delete [] data;
	data = newPointer;
	allocated = size;
	used = size;
	is_sorted = false;
}

template <class T>
void Array<T>::set_free_when_destroyed( bool f )
{
	free_when_destroyed = f;
}

template <class T>
void Array<T>::set_used( unsigned int usedNow )
{
	if (allocated < usedNow)
		reallocate(usedNow);

	used = usedNow;
}

template <class T>
void Array<T>::operator=( const Array<T>& other )
{
	if (data)
		delete [] data;

	//if (allocated < other.allocated)
	data = new T[other.allocated];

	used = other.used;
	free_when_destroyed = other.free_when_destroyed;
	is_sorted = other.is_sorted;
	allocated = other.allocated;

	for (unsigned int i=0; i<other.used; ++i)
		data[i] = other.data[i];
}

template <class T>
T& Array<T>::operator [](unsigned int index)
{
#ifdef _DEBUG
	if (index>=used)
		_asm int 3 // access violation
#endif

	return data[index];
}


template <class T>
//! Direct access operator
const T&  Array<T>::operator [](unsigned int index) const
{
#ifdef _DEBUG
	if (index>=used)
		_asm int 3 // access violation
#endif

	return data[index];
}

template <class T>
T* Array<T>::pointer()
{
	return data;
}

template <class T>
const T* Array<T>::const_pointer() const
{
	return data;
}

template <class T>
unsigned int Array<T>::size() const
{
	return used;
}

template <class T>
unsigned int Array<T>::allocated_size() const
{
	return allocated;
}

template <class T>
bool Array<T>::empty() const
{
	return used == 0;
}

template <class T>
int Array<T>::binary_search( const T& element )
{
	return binary_search(element, 0, used-1);
}

template <class T>
int Array<T>::binary_search( const T& element, int left, int right )
{
	//if (!used)
	//	return -1;

	//sort();

	//int m;

	//do
	//{
	//	m = (left+right)>>1;

	//	if (element < data[m])
	//		right = m - 1;
	//	else
	//		left = m + 1;

	//} while((element < data[m] || data[m] < element) && left<=right);

	//// this last line equals to:
	//// " while((element != array[m]) && left<=right);"
	//// but we only want to use the '<' operator.
	//// the same in next line, it is "(element == array[m])"

	//if (!(element < data[m]) && !(data[m] < element))
	//	return m;

	return -1;
}

template <class T>
void Array<T>::erase( unsigned int index )
{
#ifdef _DEBUG
	if (index>=used || index<0)
		_asm int 3 // access violation
#endif

	for (unsigned int i=index+1; i<used; ++i)
		data[i-1] = data[i];

	--used;
}

template <class T>
void Array<T>::erase( unsigned int index, int count )
{
#ifdef _DEBUG
	if (index>=used || index<0 || count<1 || index+count>used)
		_asm int 3 // access violation
#endif

	for (unsigned int i=index+count; i<used; ++i)
		data[i-count] = data[i];

	used-= count;
}

template <class T>
T& Array<T>::end()
{
#ifdef _DEBUG
	if (used<=0)
		_asm int 3 // access violation
#endif

	return data[used-1];
}

#endif