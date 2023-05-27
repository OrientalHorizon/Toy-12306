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


    template<class T>
    void sort(vector<T> &vec) {
        T *arr = new T[vec.size()];
        // MergeSort<T>(vec, 0, vec.size() - 1, arr);
        delete[] arr;
    }
    template <class T>
    void quick_sort(vector<T> &arr, const int len) {
        if (len <= 1) return;
        // 随机选择基准（pivot）
        const T pivot = arr[rand() % len];
        // i：当前操作的元素下标
        // arr[0, j)：存储小于 pivot 的元素
        // arr[k, len)：存储大于 pivot 的元素
        int i = 0, j = 0, k = len;
        // 完成一趟三路快排，将序列分为：
        // 小于 pivot 的元素 | 等于 pivot 的元素 | 大于 pivot 的元素
        while (i < k) {
            if (arr[i] < pivot)
                swap(arr[i++], arr[j++]);
            else if (pivot < arr[i])
                swap(arr[i], arr[--k]);
            else
                i++;
        }
        // 递归完成对于两个子序列的快速排序
        quick_sort(arr, j);
        quick_sort(arr + k, len - k);
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
