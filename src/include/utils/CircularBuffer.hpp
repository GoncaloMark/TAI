#pragma once

#include <memory>
#include <cstdio>
#include <stdexcept>
#include <list>

namespace cbuffer {

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
        explicit CircularBuffer<T>(size_t maxSize) {
            this->buffer = std::unique_ptr<T[]>(new T[maxSize]);
            this->maxSize = maxSize;
            this->head = 0;
            this->tail = 0;
            this->_size = 0;
        };

        // Add an item to this circular buffer.
        void enqueue(T item) {
            if (isFull()) { // if buffer is full, throw an error
                throw std::runtime_error("buffer is full");
            }

            // insert item at back of buffer
            buffer[tail] = item;
            // increment tail &  increase _size by 1
            tail = (tail + 1) % maxSize;
            _size++;
        };

        // Remove an item from this circular buffer and return it.
        T dequeue() {
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
        };

        // removes an element to insert a new one
        void update(T item) {
            (void) this->dequeue();
            this->enqueue(item);
        };

        // Return a list of all its elements
        std::vector<T> toList() {
            size_t tmp_head;
            size_t count;
            std::vector<T> items;
            for(tmp_head = head, count = 0; count < this->size(); tmp_head = (tmp_head + 1) % maxSize, count++)
            {
                items.push_back(buffer[tmp_head]);
            }
            return items;
        };

        // Return the item at the front of this circular buffer.
        T front() {
            return buffer[head];
        };

        // Return true if this circular buffer is empty, and false otherwise.
        bool isEmpty() {
            return this->size() == 0;
        };

        // Return true if this circular buffer is full, and false otherwise.
        bool isFull() {
            return _size == maxSize;
        };

        // Return the size of this circular buffer.
        size_t size() {
            return _size;
        };
    };

} // cbuffer
