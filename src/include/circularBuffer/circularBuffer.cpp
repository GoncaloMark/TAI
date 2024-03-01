//===================================================================
// File: circular_buffer.cpp
//
// Desc: A Circular Buffer implementation in C++.
//
// Copyright © 2019 Edwin Cloud. All rights reserved.
//
//===================================================================

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include "circularBuffer.hpp"


template<class T>CircularBuffer<T>::CircularBuffer(size_t maxSize) {
    this->buffer = std::unique_ptr<T[]>(new T[maxSize]);
    this->maxSize = maxSize;
    this->head = 0;
    this->tail = 0;
    this->_size = 0;
}

template<class T> void CircularBuffer<T>::enqueue(T item) {
    if (isFull()) { // if buffer is full, throw an error
        throw std::runtime_error("buffer is full");
    }

    // insert item at back of buffer
    buffer[tail] = item;
    // increment tail &  increase _size by 1
    tail = (tail + 1) % maxSize;
    _size++;
}

template<class T> T CircularBuffer<T>::dequeue() {
    // if buffer is empty, throw an error
    if (isEmpty()) {
        throw std::runtime_error("buffer is empty");
    }

    // get item at head
    T item = buffer[head];
    // set item at head to be empty
    buffer[head] = empty_item;
    // move head forward & decrease _size by 1
    head = (head + 1) % maxSize;
    _size--;

    // return item
    return item;
}

template<class T> void CircularBuffer<T>::update(T item) {
    (void) this->dequeue();
    this->enqueue(item);
}

template<class T> std::list<T> CircularBuffer<T>::toList() {
    size_t tmp_head;
    size_t count;
    std::list<T> items;
    for(tmp_head = head, count = 0; count < this->size(); tmp_head = (tmp_head + 1) % maxSize, count++)
    {
        items.push_back(buffer[tmp_head]);
    }
    return items;
}
template<class T> bool CircularBuffer<T>::isEmpty() {
    return this->size() == 0;
}

template<class T> bool CircularBuffer<T>::isFull() {
    return this->size() == maxSize;
}

template<class T> size_t CircularBuffer<T>::size() {
    return _size;
}

template<class T> T CircularBuffer<T>::front() {
    return buffer[head];
}

//---------------------------------------------------------------
// Main Function
//---------------------------------------------------------------
/**int main()
{

	CircularBuffer<uint32_t> cb(10);

	printf("\n === CircularBuffer Test ===\n");
	printf("Size: %zu\n", cb.size());

	uint32_t x = 1;
	printf("Enqueue 1, val: %d\n", x);
	cb.enqueue(x);
	printf("Size: %zu\n", cb.size());
	x = 2;
	printf("Enqueue 2, val: %d\n", x);
	cb.enqueue(x);
	printf("Size: %zu\n", cb.size());
	printf("Enqueue 3, val: %d\n", x);
	cb.enqueue(x);
	x = 4;
	printf("Enqueue 4, val: %d\n", x);
	cb.enqueue(x);
	
	std::list<uint32_t> lista = cb.toList();
	std::list<uint32_t>::iterator it;
	printf("\n");
	for(it=lista.begin(); it != lista.end(); it++)
	{
		printf("item: %u\n", *it);
	}
	printf("\n");

	printf("Size: %zu\n", cb.size());
	x = cb.dequeue();
	printf("Dequeue: %d\n", x);
	printf("Size: %zu\n", cb.size());
	x = cb.dequeue();
	printf("Dequeue: %d\n", x);
	printf("Size: %zu\n", cb.size());
	x = cb.dequeue();
	printf("Dequeue: %d\n", x);
	printf("Size: %zu\n", cb.size());
	//x = cb.dequeue();
	//printf("Dequeue: %d\n", x);
	printf("Size: %zu\n", cb.size());
	printf("Empty: %d\n", cb.isEmpty());
}**/