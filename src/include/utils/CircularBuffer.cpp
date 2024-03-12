
#include "CircularBuffer.hpp"

namespace cbuffer {

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


} // cbuffer