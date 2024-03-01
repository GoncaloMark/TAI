//===================================================================
// File: circular_buffer.hpp
//
// Desc: A Circular Buffer implementation in C++.
//
// Copyright © 2019 Edwin Cloud. All rights reserved.
//
//===================================================================
#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include <memory>
#include <cstdio>
#include <stdexcept>
#include <list>

//-------------------------------------------------------------------
// CircularBuffer (Class)
//     We will implement the buffer with a templated class so
//     the buffer can be a buffer of specified type.
//-------------------------------------------------------------------
template <class T> class CircularBuffer {

private:
	//---------------------------------------------------------------
	// CircularBuffer - Private Member Variables
	//---------------------------------------------------------------

	std::unique_ptr<T[]> buffer{}; // using a smart pointer is safer (and we don't have to implement a destructor)
	size_t head{};
	size_t tail{};
	size_t _size{};
	size_t maxSize{};

	T empty_item; // we will use this to clear data

public:
	//---------------------------------------------------------------
	// CircularBuffer - Public Methods
	//---------------------------------------------------------------

	// Create a new CircularBuffer.
	explicit CircularBuffer<T>(size_t maxSize);

	// Add an item to this circular buffer.
	void enqueue(T item);

	// Remove an item from this circular buffer and return it.
	T dequeue();

    // removes an element to insert a new one
    void update(T item);

	// Return a list of all its elements
	std::list<T> toList();

	// Return the item at the front of this circular buffer.
	T front();

	// Return true if this circular buffer is empty, and false otherwise.
	bool isEmpty();

	// Return true if this circular buffer is full, and false otherwise.
	bool isFull();

	// Return the size of this circular buffer.
	size_t size();
};

#endif /* CIRCULAR_BUFFER_H */