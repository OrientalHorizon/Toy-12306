//
// Created by iakno on 2023/5/23.
//

#ifndef TOY_12306_MAP_H
#define TOY_12306_MAP_H

/**
 * Refernces: Introduction to Algorithms, 4th ed., by CLRS: Chapter 13
 * Special Thanks to @Radioheading
 */
/*
 * 调试过程实录（不含编译错误）
 * 新建结点的时候 parent 没有给它赋值（或者是给它赋了 other.x.parent）
 * nil 这个结点是黑色的（参见算法导论），没有给它赋初值
 * 空 map 的 begin 操作和 --begin 这件事，要考虑它的鲁棒性；但是对空 map 取 begin 不需要报错，应该返回它的 end
 * Copy 里面没有拷贝相应的 nil（忘记写了）
 * 在 Node 的析构函数里面 delete val
 * 不等号能给写错了，真离谱
 * 拷贝构造函数里面 root 的爹要赋成 nil（insert 里面实现了这一点）
 * clear 要复原，把 root 赋值成 nil，clear 后要复用的！！！
 * erase 函数里面 delete 被删除的结点
 */

/*
 * 过编过程实录：
 * 注意指针和值的区别：&nil，value 里面存的元素需要 new 出来
 * const 的问题：为了指向 const map，Node * 会成为 const Node *，并且注意此时 &nil 成为了一个 const 指针
 * 但是我们要把 const 函数局限在 const_iterator 的范围内，iterator 和 const_iterator 都要调用的函数，我们可以给 const_iterator 施加 const_cast
 */

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {
    template<
            class Key,
            class T,
            class Compare = std::less<Key>
            // 注意调用 Compare 而不是默认的小于号
    > class map {
    public:
        typedef pair<const Key, T> value_type;
        enum { BLACK, RED };
        struct Node {
            value_type *val;
            Node *p, *left, *right;
            bool color; // false -- black, true -- red
            Node() : val(nullptr), p(nullptr), left(nullptr), right(nullptr), color(BLACK) {}
            Node(const Node &other) : p(nullptr), left(other.left), right(other.right), color(other.color) {
                if (other.val != nullptr) {
                    // 值要这样 copy 出来，不能直接赋指针
                    val = new value_type(*(other.val));
                }
                else {
                    val = nullptr;
                }
            }
            ~Node() {
                if (!val) return;
                delete val;
            }
        };
        Node nil;
        // 新建一个结点之后的处理，把它的默认 parent, left child, right child 都赋为 &nil
        void Proceed(Node *&node) {
            node->p = node->left = node->right = &nil;
        }

    private:
        Node *root = &nil;
        int siz; // No size_t!!!
        Compare cmp;
        bool nequal(const Key &x, const Key &y) const {
            return cmp(x, y) || cmp(y, x);
        }

        Node *Search(Node *x, const Key &k) const {
            while (x != &nil && nequal(k, x->val->first)) {
                if (cmp(k, x->val->first)) {
                    x = x->left;
                }
                else {
                    x = x->right;
                }
            }
            return x;
        }
        Node *GetMin(Node *x) const {
            while (x->left && x->left != &nil) {
                x = x->left;
            }
            return x;
        }
        Node *GetMax(Node *x) const {
            while (x->right != &nil) {
                x = x->right;
            }
            return x;
        }

        Node *Successor(Node *x) const {
            if (x->right != &nil) {
                // 右子树一直走左孩子
                return GetMin(x->right);
            }
            else {
                // 找到第一个靠右侧的父亲
                Node *y = x->p;
                while (y != &nil && x == y->right) {
                    x = y;
                    y = y->p;
                }
                return y;
            }
        }
        Node *Predecessor(Node *x) const {
            // 对称的
            if (x->left != &nil) {
                return GetMax(x->left);
            }
            else {
                Node *y = x->p;
                while (y != &nil && x == y->left) {
                    x = y;
                    y = y->p;
                }
                return y;
            }
        }

        void LeftRotate(Node *x) {
            Node *y = x->right;
            x->right = y->left; // beta
            if (y->left != &nil) {
                y->left->p = x;
            }
            y->p = x->p;
            if (x->p == &nil) {
                root = y;
            }
            else if (x == x->p->left) {
                y->p->left = y;
            }
            else {
                y->p->right = y;
            }
            y->left = x;
            x->p = y;
        }
        void RightRotate(Node *x) {
            Node *y = x->left;
            x->left = y->right;
            if (y->right != &nil) {
                y->right->p = x;
            }
            y->p = x->p;
            if (x->p == &nil) {
                root = y;
            }
            else if (x == x->p->right) {
                y->p->right = y;
            }
            else {
                y->p->left = y;
            }
            y->right = x;
            x->p = y;
        }

        void InternalInsertFixup(Node *z) {
            while (z->p->color == RED) {
                if (z->p == z->p->p->left) {
                    Node *y = z->p->p->right; // y is z.p's sibling，考察 y 的性质
                    if (y->color == RED) {
                        // Case 1: 让 z 和 z.p 这一层能够满足，然后迭代
                        z->p->color = BLACK;
                        y->color = BLACK;
                        z->p->p->color = RED;
                        z = z->p->p;
                    }
                    else {
                        if (z == z->p->right) {
                            // Case 2: 直接转化为 3
                            z = z->p;
                            LeftRotate(z);
                        }
                        RightRotate(z->p->p);
                        z->p->color = BLACK;
                        z->p->right->color = RED;
                        break;
                    }
                }
                else {
                    // symmetric
                    Node *y = z->p->p->left;
                    if (y->color == RED) {
                        z->p->color = BLACK;
                        y->color = BLACK;
                        z->p->p->color = RED;
                        z = z->p->p;
                    }
                    else {
                        if (z == z->p->left) {
                            z = z->p;
                            RightRotate(z);
                        }
                        LeftRotate(z->p->p);
                        z->p->color = BLACK;
                        z->p->left->color = RED;
                        break;
                    }
                }
            }
            root->color = BLACK;
        }
        Node *InternalInsert(Node *z) {
            Node *x = root, *y = &nil;
            // find z's father (named y)
            while (x != &nil) {
                y = x;
                if (cmp(z->val->first, x->val->first)) {
                    x = x->left;
                }
                else {
                    x = x->right;
                }
            }
            z->p = y;
            if (y == &nil) {
                root = z;
            }
            else if (cmp(z->val->first, y->val->first)) {
                y->left = z;
            }
            else {
                y->right = z;
            }
            z->left = z->right = &nil;
            z->color = RED;
            if (z->p->color == RED) {
                InternalInsertFixup(z);
            }
            root->color = BLACK;
            return z;
        }

        void Replace(Node *u, Node *v) {
            // 移花接木，v 替换 u
            if (u->p == &nil) {
                root = v;
            }
            else if (u == u->p->left) {
                u->p->left = v;
            }
            else {
                u->p->right = v;
            }
            v->p = u->p;
        }
        void InternalDeleteFixup(Node *x) {
            while (x != root && x->color == BLACK) {
                if (x == x->p->left) {
                    Node *w = x->p->right; // w is x's sibling
                    if (w->color == RED) {
                        // Case 1: to cases 2, 3, 4
                        w->color = BLACK;
                        x->p->color = RED;
                        LeftRotate(x->p);
                        w = x->p->right;
                    }
                    if (w->left->color == BLACK && w->right->color == BLACK) {
                        w->color = RED;
                        x = x->p;
                    }
                    else {
                        if (w->right->color == BLACK) {
                            w->left->color = BLACK;
                            w->color = RED;
                            RightRotate(w);
                            w = x->p->right;
                        }
                        w->color = x->p->color;
                        x->p->color = BLACK;
                        w->right->color = BLACK;
                        LeftRotate(x->p);
                        root->color = BLACK;
                        return;
                    }
                }
                else {
                    Node *w = x->p->left; // w is x's sibling
                    if (w->color == RED) {
                        w->color = BLACK;
                        x->p->color = RED;
                        RightRotate(x->p);
                        w = x->p->left;
                    }
                    if (w->right->color == BLACK && w->left->color == BLACK) {
                        w->color = RED;
                        x = x->p;
                    }
                    else {
                        if (w->left->color == BLACK) {
                            w->right->color = BLACK;
                            w->color = RED;
                            LeftRotate(w);
                            w = x->p->left;
                        }
                        w->color = x->p->color;
                        x->p->color = BLACK;
                        w->left->color = BLACK;
                        RightRotate(x->p);
                        root->color = BLACK;
                        return;
                    }
                }
            }
            x->color = BLACK;
        }
        void InternalDelete(Node *z) {
            Node *x = &nil, *y = z;
            bool original_color = y->color;
            if (z->left == &nil) {
                x = z->right;
                Replace(z, x);
            }
            else if (z->right == &nil) {
                x = z->left;
                Replace(z, x);
            }
            else {
                y = GetMin(z->right); // successor
                original_color = y->color;
                x = y->right;
                if (y != z->right) {
                    // 在找后继的过程中还要往左
                    Replace(y, y->right);
                    y->right = z->right;
                    y->right->p = y;
                }
                else {
                    x->p = y;
                }
                Replace(z, y);
                y->left = z->left;
                y->left->p = y;
                y->color = z->color;
            }
            if (original_color == BLACK) {
                InternalDeleteFixup(x);
            }
        }

        void Copy(Node *x, Node *y, Node *xnil, const Node *ynil) {
            // 为什么要 const Node *：拷贝构造和赋值中 other 是 const map &，所以 &(other.nil) 是一个 const 指针
            if (y == ynil) {
                x = xnil;
                return;
            }
            if (y->left == ynil) {
                x->left = xnil;
            }
            else {
                x->left = new Node(*(y->left));
                x->left->p = x;
                Copy(x->left, y->left, xnil, ynil);
            }
            if (y->right == ynil) {
                x->right = xnil;
            }
            else {
                x->right = new Node(*(y->right));
                x->right->p = x;
                Copy(x->right, y->right, xnil, ynil);
            }
        }
        // delete 时要特判 nil，防止 nil 重复删除
        void Delete(Node *x) {
            if (x == &nil) {
                return;
            }
            if (x->left != &nil) {
                Delete(x->left);
            }
            if (x->right != &nil) {
                Delete(x->right);
            }
            delete x;
        }
    public:
        class const_iterator;
        class iterator {
            friend class map;
        private:
            const map<Key, T, Compare> *cur;
            Node *ptr;
        public:
            iterator(const map *curr = nullptr, Node *ptrr = nullptr) : cur(curr), ptr(ptrr) {}
            iterator(const iterator &other) : cur(other.cur), ptr(other.ptr) {}

            iterator operator++(int) {
                if (ptr == nullptr || cur == nullptr || ptr == &(cur->nil)) {
                    throw invalid_iterator();
                }
                iterator ret(*this);
                ptr = cur->Successor(ptr);
                return ret;
            }
            iterator &operator++() {
                if (ptr == nullptr || cur == nullptr || ptr == &(cur->nil)) {
                    throw invalid_iterator();
                }
                ptr = cur->Successor(ptr);
                return *this;
            }
            // nil 的 -- 要照顾到
            iterator operator--(int) {
                if (ptr == nullptr || cur == nullptr || ptr == cur->GetMin(cur->root)) {
                    throw invalid_iterator();
                }
                if (ptr == &(cur->nil)) {
                    iterator ret(*this);
                    ptr = cur->GetMax(cur->root);
                    return ret;
                }
                iterator ret(*this);
                ptr = cur->Predecessor(ptr);
                return ret;
            }
            iterator &operator--() {
                if (ptr == nullptr || cur == nullptr || ptr == cur->GetMin(cur->root)) {
                    throw invalid_iterator();
                }
                if (ptr == &(cur->nil)) {
                    ptr = cur->GetMax(cur->root);
                    return *this;
                }
                ptr = cur->Predecessor(ptr);
                return *this;
            }

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
            // 不等号居然写错了！！注意代码重用！
            bool operator!=(const iterator &rhs) const {
                return !((*this) == rhs);
            }
            bool operator!=(const const_iterator &rhs) const {
                return !((*this) == rhs);
            }

            value_type &operator*() const {
                if (ptr == nullptr || cur == nullptr || ptr == &(cur->nil)) {
                    throw invalid_iterator();
                }
                return *(ptr->val);
            }
            value_type* operator->() const noexcept {
                if (ptr == nullptr || cur == nullptr || ptr == &(cur->nil)) {
                    throw invalid_iterator();
                }
                return ptr->val;
            }
        };
        class const_iterator {
            friend class map;
        private:
            const map<Key, T, Compare> *cur;
            const Node *ptr;
        public:
            const_iterator(const map *curr = nullptr, const Node *ptrr = nullptr) : cur(curr), ptr(ptrr) {}
            const_iterator(const map *curr, Node *ptrr) : cur(curr), ptr(ptrr) {}
            const_iterator(const const_iterator &other) : cur(other.cur), ptr(other.ptr) {}
            const_iterator(const iterator &other) : cur(other.cur), ptr(other.ptr) {}

            const_iterator operator++(int) {
                if (ptr == nullptr || cur == nullptr || ptr == &(cur->nil)) {
                    throw invalid_iterator();
                }
                const_iterator ret(*this);
                ptr = cur->Successor(const_cast<Node *>(ptr));
                return ret;
            }
            const_iterator &operator++() {
                if (ptr == nullptr || cur == nullptr || ptr == &(cur->nil)) {
                    throw invalid_iterator();
                }
                ptr = cur->Successor(const_cast<Node *>(ptr));
                return *this;
            }
            const_iterator operator--(int) {
                if (ptr == nullptr || cur == nullptr || ptr == cur->GetMin(cur->root)) {
                    throw invalid_iterator();
                }
                if (ptr == &(cur->nil)) {
                    const_iterator ret(*this);
                    ptr = cur->GetMax(cur->root);
                    return ret;
                }
                const_iterator ret(*this);
                ptr = cur->Predecessor(const_cast<Node *>(ptr));
                return ret;
            }
            const_iterator &operator--() {
                if (ptr == nullptr || cur == nullptr || ptr == cur->GetMin(cur->root)) {
                    throw invalid_iterator();
                }
                if (ptr == &(cur->nil)) {
                    ptr = cur->GetMax(cur->root);
                    return *this;
                }
                ptr = cur->Predecessor(const_cast<Node *>(ptr));
                return *this;
            }

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
            bool operator!=(const iterator &rhs) const {
                return !((*this) == rhs);
            }
            bool operator!=(const const_iterator &rhs) const {
                return !((*this) == rhs);
            }

            value_type &operator*() const {
                if (ptr == nullptr || cur == nullptr || ptr == &(cur->nil)) {
                    throw invalid_iterator();
                }
                return *(ptr->val);
            }
            value_type* operator->() const noexcept {
                if (ptr == nullptr || cur == nullptr || ptr == &(cur->nil)) {
                    throw invalid_iterator();
                }
                return ptr->val;
            }
        };

        map() {
            root = &nil;
            siz = 0;
        }
        map(const map &other) {
            if (this == &other) {
                return;
            }
            siz = other.siz;
            if (other.root != &other.nil) {
                root = new Node(*(other.root));
                root->p = &nil;
                Copy(root, other.root, &nil, &(other.nil));
            }
        }
        map & operator=(const map &other) {
            if (this == &other) {
                return *this;
            }
            Delete(root);
            if (other.root != &other.nil) {
                root = new Node(*(other.root));
                root->p = &nil;
                Copy(root, other.root, &nil, &(other.nil));
            }
            siz = other.siz;
            return *this;
        }
        ~map() {
            Delete(root);
            siz = 0;
        }

        T & at(const Key &key) {
            Node *x = Search(root, key);
            if (x == &nil) {
                throw index_out_of_bound();
            }
            return x->val->second;
        }
        const T & at(const Key &key) const {
            Node *x = Search(root, key);
            if (x == &nil) {
                throw index_out_of_bound();
            }
            return x->val->second;
        }
        T & operator[](const Key &key) {
            Node *x = Search(root, key);
            if (x == &nil) {
                Node *y = new Node();
                Proceed(y);
                value_type *newVal = new pair<const Key, T>(key, T());
                y->val = newVal;
                InternalInsert(y);
                ++siz;
                return y->val->second;
            }
            return x->val->second;
        }
        const T & operator[](const Key &key) const {
            Node *x = Search(root, key);
            if (x == &nil) {
                throw index_out_of_bound();
            }
            return x->val->second;
        }

        // 空 map 的 begin 不需要报错，返回 nil 即可
        iterator begin() {
//		if (root == nullptr || root == &nil) {
//			throw container_is_empty();
//		}
            Node *x = GetMin(root);
            return iterator(this, x);
        }
        const_iterator cbegin() const {
//		if (root == nullptr || root == &nil) {
//			throw container_is_empty();
//		}
            Node *x = GetMin(root);
            return const_iterator(this, x);
        }
        iterator end() {
            return iterator(this, &nil);
        }
        const_iterator cend() const {
            return const_iterator(this, &nil);
        }

        bool empty() const {
            return (siz == 0);
        }

        size_t size() const {
            return siz;
        }

        void clear() {
            // 跑一遍深搜
            Delete(root);
            siz = 0;
            root = &nil; // 注意复原！
        }

        pair<iterator, bool> insert(const value_type &value) {
            Node *x = Search(root, value.first);
            if (x == &nil) {
                Node *z = new Node();
                Proceed(z);
                ++siz;
                value_type *newVal = new value_type(value);
                z->val = newVal;
                InternalInsert(z);
                return pair<iterator, bool>(iterator(this, z), true);
            }
            return pair<iterator, bool>(iterator(this, x), false);
        }
        void erase(iterator pos) {
            if (pos.cur != this || pos.ptr == &nil || pos.ptr == nullptr) {
                throw invalid_iterator();
            }
            --siz;
            InternalDelete(pos.ptr);
            delete(pos.ptr);
        }

        size_t count(const Key &key) const {
            Node *x = Search(root, key);
            if (x == &nil) return 0u;
            return 1u;
        }
        iterator find(const Key &key) {
            Node *x = Search(root, key);
            if (x == &nil) return end();
            return iterator(this, x);
        }
        const_iterator find(const Key &key) const {
            Node *x = Search(root, key);
            if (x == &nil) return cend();
            return const_iterator(this, x);
        }
    };

}

#endif //TOY_12306_MAP_H
