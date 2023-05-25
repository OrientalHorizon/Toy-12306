//
// Created by iakno on 2023/5/23.
//

#ifndef TOY_12306_VECTOR_H
#define TOY_12306_VECTOR_H

#include "exceptions.hpp"
#include "utility.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
    template<typename T>
    class vector
    {
    private:
        size_t Size, Capacity;
        T **Obj;

        void DoubleSpace() {
            Capacity <<= 1;
            T** tmp = new T*[Capacity];
            for (size_t i = 0; i < Size; ++i) {
                tmp[i] = new T(*Obj[i]);
                delete Obj[i];
            }
            std::swap(tmp, Obj);
            delete[] tmp;
        }

    public:
        /**
         * TODO
         * a type for actions of the elements of a vector, and you should write
         *   a class named const_iterator with same interfaces.
         */
        /**
         * you can see RandomAccessIterator at CppReference for help.
         */
        class iterator;
        class const_iterator;
        friend class iterator;
        friend class const_iterator;
        class iterator
        {
            // The following code is written for the C++ type_traits library.
            // Type traits is a C++ feature for describing certain properties of a type.
            // For instance, for an iterator, iterator::value_type is the type that the
            // iterator points to.
            // STL algorithms and containers may use these type_traits (e.g. the following
            // typedef) to work properly. In particular, without the following code,
            // @code{std::sort(iter, iter1);} would not compile.
            // See these websites for more information:
            // https://en.cppreference.com/w/cpp/header/type_traits
            // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
            // About iterator_category: https://en.cppreference.com/w/cpp/iterator
            friend class vector;
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::output_iterator_tag;

        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            size_t ptr;
            vector* cur;
        public:
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            iterator() : ptr(-1), cur(nullptr) {}
            iterator(size_t tmpPtr, vector* tmpCur) {
                ptr = tmpPtr;
                cur = tmpCur;
            }
            iterator(const iterator &other) {
                ptr = other.ptr;
                cur = other.cur;
            }
            iterator operator+(const int &n) const
            {
                //TODO
                iterator tmp(ptr + n, cur);
                return tmp;
            }
            iterator operator-(const int &n) const
            {
                //TODO
                iterator tmp(ptr - n, cur);
                return tmp;
            }
            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const
            {
                //TODO
                if (cur != rhs.cur || cur == nullptr || rhs.cur == nullptr) {
                    throw invalid_iterator();
                }
                return abs(rhs.ptr - ptr);
            }
            iterator& operator+=(const int &n)
            {
                //TODO
                ptr += n;
                return *this;
            }
            iterator& operator-=(const int &n)
            {
                //TODO
                ptr -= n;
                return *this;
            }
            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator tmp(*this);
                ptr++;
                return tmp;
            }
            /**
             * TODO ++iter
             */
            iterator& operator++() {
                ptr++;
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator tmp(*this);
                ptr--;
                return tmp;
            }
            /**
             * TODO --iter
             */
            iterator& operator--() {
                ptr--;
                return *this;
            }
            /**
             * TODO *it
             */
            T& operator*() const {
                if (cur == nullptr) {
                    throw invalid_iterator();
                }
                if (ptr < 0 || ptr >= cur->Size) {
                    throw invalid_iterator();
                }
                return (*cur)[ptr];
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                if (cur != rhs.cur) {
                    return false;
                }
                return (ptr == rhs.ptr);
            }
            bool operator==(const const_iterator &rhs) const {
                if (cur != rhs.cur) {
                    return false;
                }
                return (ptr == rhs.ptr);
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }
            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };
        /**
         * TODO
         * has same function as iterator, just for a const object.
         */
        class const_iterator
        {
            friend class vector;
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::output_iterator_tag;

        private:
            /*TODO*/
            size_t ptr;
            const vector* cur;

        public:
            const_iterator() : ptr(-1), cur(nullptr) {}
            const_iterator(size_t tmpPtr, const vector* tmpCur) {
                ptr = tmpPtr;
                cur = tmpCur;
            }
            const_iterator(const const_iterator &other) {
                ptr = other.ptr;
                cur = other.cur;
            }
            const_iterator operator+(const int &n) const
            {
                //TODO
                const_iterator tmp(ptr + n, cur);
                return tmp;
            }
            const_iterator operator-(const int &n) const
            {
                //TODO
                const_iterator tmp(ptr - n, cur);
                return tmp;
            }
            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const const_iterator &rhs) const
            {
                //TODO
                if (cur != rhs.cur || cur == nullptr || rhs.cur == nullptr) {
                    throw invalid_iterator();
                }
                return abs(rhs.ptr - ptr);
            }
            const_iterator& operator+=(const int &n)
            {
                //TODO
                ptr += n;
                return *this;
            }
            const_iterator& operator-=(const int &n)
            {
                //TODO
                ptr -= n;
                return *this;
            }
            /**
             * TODO iter++
             */
            const_iterator operator++(int) {
                iterator tmp(*this);
                ptr++;
                return tmp;
            }
            /**
             * TODO ++iter
             */
            const_iterator& operator++() {
                ptr++;
                return *this;
            }
            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                iterator tmp(*this);
                ptr--;
                return tmp;
            }
            /**
             * TODO --iter
             */
            const_iterator& operator--() {
                ptr--;
                return *this;
            }
            /**
             * TODO *it
             */
            const T& operator*() const {
                if (cur == nullptr) {
                    throw invalid_iterator();
                }
                if (ptr < 0 || ptr >= cur->Size) {
                    throw invalid_iterator();
                }
                return (*cur)[ptr];
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                if (cur != rhs.cur) {
                    return false;
                }
                return (ptr == rhs.ptr);
            }
            bool operator==(const const_iterator &rhs) const {
                if (cur != rhs.cur) {
                    return false;
                }
                return (ptr == rhs.ptr);
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }
            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };
        /**
         * TODO Constructs
         * At least two: default constructor, copy constructor
         */
        vector() : Size(0), Capacity(8), Obj(nullptr) {
            Obj = new T*[Capacity];
        }
        vector(const vector &other) : Size(other.Size), Capacity(other.Capacity), Obj(nullptr) {
            Obj = new T*[Capacity];
            for (size_t i = 0; i < Size; ++i) {
                Obj[i] = new T(*other.Obj[i]);
            }
        }
        /**
         * TODO Destructor
         */
        ~vector() {
            for (size_t i = 0; i < Size; ++i) {
                delete Obj[i];
            }
            delete[] Obj;
        }
        /**
         * TODO Assignment operator
         */
        vector &operator=(const vector &other) {
            if (this == &other) {
                return *this;
            }
            for (size_t i = 0; i < Size; ++i) {
                delete Obj[i];
            }
            delete[] Obj;
            Size = other.Size, Capacity = other.Capacity;
            Obj = new T*[Capacity];
            for (size_t i = 0; i < Size; ++i) {
                Obj[i] = new T(*other.Obj[i]);
            }
            return *this;
        }
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         */
        T & at(const size_t &pos) {
            if (pos < 0 || pos >= Size) {
                throw index_out_of_bound();
            }
            return *Obj[pos];
        }
        const T & at(const size_t &pos) const {
            if (pos < 0 || pos >= Size) {
                throw index_out_of_bound();
            }
            return *Obj[pos];
        }
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */
        T & operator[](const size_t &pos) {
            if (pos < 0 || pos >= Size) {
                throw index_out_of_bound();
            }
            return *Obj[pos];
        }
        const T & operator[](const size_t &pos) const {
            if (pos < 0 || pos >= Size) {
                throw index_out_of_bound();
            }
            return *Obj[pos];
        }
        /**
         * access the first element.
         * throw container_is_empty if size == 0
         */
        const T & front() const {
            if (Size == 0) {
                throw container_is_empty();
            }
            return *Obj[0];
        }
        /**
         * access the last element.
         * throw container_is_empty if size == 0
         */
        const T & back() const {
            if (Size == 0) {
                throw container_is_empty();
            }
            return *Obj[Size - 1];
        }
        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            iterator tmp(0, this);
            return tmp;
        }
        const_iterator cbegin() const {
            const_iterator tmp(0, this);
            return tmp;
        }
        /**
         * returns an iterator to the end.
         */
        iterator end() {
            iterator tmp(this->Size, this);
            return tmp;
        }
        const_iterator cend() const {
            const_iterator tmp(this->Size, this);
            return tmp;
        }
        /**
         * checks whether the container is empty
         */
        bool empty() const {
            return (!Size);
        }
        /**
         * returns the number of elements
         */
        size_t size() const {
            return Size;
        }
        /**
         * clears the contents
         */
        void clear() {
            for (size_t i = 0; i < Size; ++i) {
                delete Obj[i];
            }
            Size = 0;
        }
        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
        iterator insert(iterator pos, const T &value) {
            if (pos.cur != this) {
                throw invalid_iterator();
            }
            if (Size == Capacity) {
                DoubleSpace();
            }
            const size_t DIFF = 100;
            for (size_t i = Size + DIFF - 1; i >= DIFF + pos.ptr; --i) {
                size_t j = i - DIFF;
                Obj[j + 1] = Obj[j];
            }
            Obj[pos.ptr] = new T(value);
            ++Size;
            iterator tmp(pos.ptr, this);
            return tmp;
        }
        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
         */
        iterator insert(const size_t &ind, const T &value) {
            if (ind > Size) {
                throw index_out_of_bound();
            }
            if (Size == Capacity) {
                DoubleSpace();
            }
            const size_t DIFF = 100;
            for (size_t i = Size + DIFF - 1; i >= DIFF + ind; --i) {
                size_t j = i - DIFF;
                Obj[j + 1] = Obj[j];
            }
            Obj[ind] = new T(value);
            ++Size;
            iterator tmp(ind, this);
            return tmp;
        }
        /**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
        iterator erase(iterator pos) {
            if (pos.cur != this) {
                throw invalid_iterator();
            }
            delete Obj[pos.ptr];
            const size_t DIFF = 100;
            for (size_t i = pos.ptr + DIFF; i < Size + DIFF - 1; ++i) {
                Obj[i - DIFF] = Obj[i - DIFF + 1];
            }
            Obj[Size] = nullptr;
            --Size;
            if (pos.ptr == Size) {
                return this->end();
            }
            iterator tmp(pos.ptr, this);
            return tmp;
        }
        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= size
         */
        iterator erase(const size_t &ind) {
            if (ind >= Size) {
                throw index_out_of_bound();
            }
            delete Obj[ind];
            const size_t DIFF = 100;
            for (size_t i = ind + DIFF; i < Size + DIFF - 1; ++i) {
                Obj[i - DIFF] = Obj[i + 1 - DIFF];
            }
            Obj[Size] = nullptr;
            --Size;
            if (ind == Size) {
                return this->end();
            }
            iterator tmp(ind, this);
            return tmp;
        }
        /**
         * adds an element to the end.
         */
        void push_back(const T &value) {
            if (Size == Capacity) {
                DoubleSpace();
            }
            ++Size;
            Obj[Size - 1] = new T(value);
        }
        /**
         * remove the last element from the end.
         * throw container_is_empty if size() == 0
         */
        void pop_back() {
            if (!Size) {
                throw container_is_empty();
            }
            delete Obj[Size - 1];
            --Size;
        }
    };


}

#endif //TOY_12306_VECTOR_H
