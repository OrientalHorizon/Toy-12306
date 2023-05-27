//
// Created by iakno on 2023/5/23.
//

#ifndef TOY_12306_UTILS_H
#define TOY_12306_UTILS_H

#include <iostream>
#include <utility>
#include <string>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <cassert>
#include "map.h"
#include "vector.h"
#include "exceptions.h"

namespace sjtu {
    template<class T>
    int lower_bound(const T *arr, int l, int r, T x) {
        while (l < r) {
            int mid = (l + r) >> 1;
            if (arr[mid] < x) l = mid + 1;
            else r = mid;
        }
        return l;
    }

    template<class T>
    int upper_bound(const T *arr, int l, int r, T x) {
        while (l < r) {
            int mid = (l + r) >> 1;
            if (arr[mid] <= x) l = mid + 1;
            else r = mid;
        }
        return l;
    }

    template<class T, class Compare = std::less<T>>
    void MergeSort(vector<T> &vec, int l, int r, T *arr) {
        if (l == r) {
            return;
        }
        int mid = (l + r) >> 1;
        MergeSort(vec, l, mid, arr);
        MergeSort(vec, mid + 1, r, arr);
        int i = l, j = mid + 1, k = l;
        while (i <= mid && j <= r) {
            if (Compare{}(vec[i], vec[j])) {
                arr[k++] = vec[i++];
            }
            else {
                arr[k++] = vec[j++];
            }
        }
        while (i <= mid) {
            arr[k++] = vec[i++];
            // ans += (long long)(mid - i + 1);
        }
        while (j <= r) {
            arr[k++] = vec[j++];
        }
        for (int i = l; i <= r; ++i) {
            vec[i] = arr[i];
        }
    }
    template<class T>
    void sort(vector<T> &vec) {
        T *arr = new T[vec.size()];
        MergeSort<T>(vec, 0, vec.size() - 1, arr);
        delete[] arr;
    }
}

struct MyID {
    char s[21];
    MyID() : s("") {}
    MyID(const MyID &others) {
        strcpy(s, others.s);
    }
    MyID(const std::string &str) {
        assert(str.size() <= 20);
        strcpy(s, str.c_str());
    }
    MyID &operator=(const MyID &others) {
        strcpy(s, others.s);
        return *this;
    }
    inline bool operator==(const MyID &y) const {
        return !strcmp(s, y.s);
    }
    inline bool operator<(const MyID &y) const {
        return strcmp(s, y.s) < 0;
    }
    inline bool operator<=(const MyID &y) const {
        return strcmp(s, y.s) <= 0;
    }
};
std::ostream &operator<<(std::ostream &os, const MyID &id) {
    os << id.s;
    return os;
}

struct MyString {
    char s[31];
    MyString() : s("") {}
    MyString(const MyString &others) {
        strcpy(s, others.s);
    }
    MyString(const std::string &str) {
        assert(str.size() <= 30);
        strcpy(s, str.c_str());
    }
    MyString &operator=(const MyString &others) {
        strcpy(s, others.s);
        return *this;
    }
    inline bool operator==(const MyString &y) const {
        return !strcmp(s, y.s);
    }
    inline bool operator<(const MyString &y) const {
        return strcmp(s, y.s) < 0;
    }
    inline bool operator<=(const MyString &y) const {
        return strcmp(s, y.s) <= 0;
    }
};
std::ostream &operator<<(std::ostream &os, const MyString &str) {
    os << str.s;
    return os;
}

#endif //TOY_12306_UTILS_H
