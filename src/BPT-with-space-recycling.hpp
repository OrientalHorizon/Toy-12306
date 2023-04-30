#ifndef BPlus_TREE
#define BPlus_TREE
#include <iostream>
#include <fstream>
#include <cstring>

// Modified on 2023/4/30
// 最后一次更改：二分后边界条件特判

using std::cin;
using std::cout;
using std::endl;

class myString {
private:
    char s[66];
public:
    myString() = default;
    myString(const char* t) {
        strcpy(s, t);
    }
    myString &operator=(const myString &x) {
        // assert(strlen(x.s) <= 64);
        if (this == &x) return *this;
        strcpy(s, x.s);
        return *this;
    }
    friend bool operator==(const myString &x, const myString &y);
    friend bool operator<(const myString &x, const myString &y);
    friend std::ostream& operator<<(std::ostream &x, const myString &y);
};
bool operator==(const myString &x, const myString &y) {
    return (!strcmp(x.s, y.s));
}
bool operator!=(const myString &x, const myString &y) {
    return !(x == y);
}
bool operator<(const myString &x, const myString &y) {
    return (strcmp(x.s, y.s) < 0);
}

bool operator>(const myString &x, const myString &y) {
    return (y < x);
}
bool operator<=(const myString &x, const myString &y) {
    return !(x > y);
}

bool operator>=(const myString &x, const myString &y) {
    return !(x < y);
}
std::ostream& operator<<(std::ostream &x, const myString &y) {
    x << y.s;
    return x;
}

template<class keyType, class valueType>
struct Element {
    keyType str;
    valueType val;
    Element() = default;
    Element(const keyType &t, const valueType &v) {
        str = t;
        val = v;
    }
    Element &operator=(const Element &x) {
        if (this == &x) return *this;
        str = x.str;
        val = x.val;
        return *this;
    }
};
template<class keyType, class valueType>
bool operator==(const Element<keyType, valueType> &a, const Element<keyType, valueType> &b) {
    if (!(a.str == b.str)) {
        return false;
    }
    return a.val == b.val;
}
template<class keyType, class valueType>
bool operator!=(const Element<keyType, valueType> &a, const Element<keyType, valueType> &b) {
    return !(a == b);
}
template<class keyType, class valueType>
bool operator<(const Element<keyType, valueType> &a, const Element<keyType, valueType> &b) {
    if (!(a.str == b.str)) {
        return (a.str < b.str);
    }
    return a.val < b.val;
}
template<class keyType, class valueType>
bool operator>(const Element<keyType, valueType> &a, const Element<keyType, valueType> &b) {
    if (!(a.str == b.str)) {
        return !(a.str < b.str);
    }
    return a.val > b.val;
}
template<class keyType, class valueType>
bool operator<=(const Element<keyType, valueType> &a, const Element<keyType, valueType> &b) {
    return !(a > b);
}
template<class keyType, class valueType>
bool operator>=(const Element<keyType, valueType> &a, const Element<keyType, valueType> &b) {
    return !(a < b);
}

const int MaxSize = 120, MinSize = 60;
template<class keyType, class valueType>
struct Block {
    int isLeaf = 0; // 是否是叶结点
    int siz = 0;
    int nxt = -1; // 如果它是叶结点，那么它的下一个块是谁（方便从底层遍历）
    // 注意指针个数会比元素个数多一个
    // 这里记录元素个数！！！！！
    Element<keyType, valueType> ele[MaxSize + 2];
    int chd[MaxSize + 3]; // 下属的块在哪个位置
};

template<class keyType, class valueType>
class BPlusTree {
private:
    int nowsize = -1; // 最后一个块的位置
    std::fstream _file;
    std::string _filename;
    int recycle[5001];
    void rdall(int pos, Block<keyType, valueType> &blk) {
        if (pos < 0) exit(-1);
        _file.seekg(pos * sizeof(Block<keyType, valueType>) + sizeof(int) * 2 + sizeof(recycle));
        _file.read(reinterpret_cast<char *>(&blk), sizeof(blk));
    }
    void wtall(int pos, const Block<keyType, valueType> &blk) {
        if (pos < 0) exit(-1);
        _file.seekp(pos * sizeof(Block<keyType, valueType>) + sizeof(int) * 2 + sizeof(recycle));
        _file.write(reinterpret_cast<const char *>(&blk), sizeof(blk));
    }
    void rdfirst(int pos, Block<keyType, valueType> &blk) {
        if (pos < 0) exit(-1);
        _file.seekg(pos * sizeof(Block<keyType, valueType>) + sizeof(int) * 2 + sizeof(recycle));
        _file.read(reinterpret_cast<char *>(&(blk.isLeaf)), sizeof(blk.isLeaf));
        _file.read(reinterpret_cast<char *>(&(blk.siz)), sizeof(blk.siz));
        _file.read(reinterpret_cast<char *>(&(blk.nxt)), sizeof(blk.nxt));
        _file.read(reinterpret_cast<char *>(&(blk.ele)), sizeof(blk.ele));
    }
    void wtfirst(int pos, const Block<keyType, valueType> &blk) {
        if (pos < 0) exit(-1);
        _file.seekp(pos * sizeof(Block<keyType, valueType>) + sizeof(int) * 2 + sizeof(recycle));
        _file.write(reinterpret_cast<const char *>(&(blk.isLeaf)), sizeof(blk.isLeaf));
        _file.write(reinterpret_cast<const char *>(&(blk.siz)), sizeof(blk.siz));
        _file.write(reinterpret_cast<const char *>(&(blk.nxt)), sizeof(blk.nxt));
        _file.write(reinterpret_cast<const char *>(&(blk.ele)), sizeof(blk.ele));
    }
    void rdsize(int pos, Block<keyType, valueType> &blk) {
        if (pos < 0) exit(-1);
        _file.seekg(pos * sizeof(Block<keyType, valueType>) + sizeof(int) * 3 + sizeof(recycle));
        _file.read(reinterpret_cast<char *>(&(blk.siz)), sizeof(blk.siz));
    }
    void wtsize(int pos, const Block<keyType, valueType> &blk) {
        if (pos < 0) exit(-1);
        _file.seekp(pos * sizeof(Block<keyType, valueType>) + sizeof(int) * 3 + sizeof(recycle));
        _file.write(reinterpret_cast<const char *>(&(blk.siz)), sizeof(blk.siz));
    }

    int last = -1;
    int GetPos() {
        if (recycle[0]) {
            int ret = recycle[recycle[0]--];
            last = ret;
            return ret;
        }
        last = (++nowsize);
        return nowsize;
    }
    inline int LastPos() {
        return last;
    }
    inline void Recycle(int num) {
        recycle[++recycle[0]] = num;
    }

    Element<keyType, valueType> pass;
    bool InternalInsert(Block<keyType, valueType> &cur, int pos, const Element<keyType, valueType> &ele) {
        // 注意比最后一个元素大要不要特判
        if (cur.isLeaf) {
            int l = 0, r = cur.siz;
            while (l < r) {
                int mid = (l + r) >> 1;
                if (cur.ele[mid] > ele) {
                    r = mid;
                }
                else {
                    l = mid + 1;
                }
            }
            if (l > 0 && cur.ele[l - 1] == ele) {
                // 插入失败
                return false;
            }
            // 插在 l 处
            if (cur.siz < MaxSize) {
                for (int i = cur.siz - 1; i >= l; --i) {
                    cur.ele[i + 1] = cur.ele[i];
                }
                ++cur.siz;
                cur.ele[l] = ele;
                wtfirst(pos, cur);
                // 可不可能出现要调整头顶上值的情况？
                // 貌似不会
                return false; // 不调整
            }

            // 裂开！
            for (int i = cur.siz - 1; i >= l; --i) {
                cur.ele[i + 1] = cur.ele[i];
            }
            ++cur.siz;
            cur.ele[l] = ele;
            int newpos = GetPos();
            static Block<keyType, valueType> blk;
            blk.isLeaf = true;
            blk.siz = MinSize + 1;
            blk.nxt = cur.nxt;
            cur.nxt = newpos;
            for (int i = 0; i <= MinSize; ++i) {
                blk.ele[i] = cur.ele[i + MinSize];
            }
            cur.siz = MinSize;
            if (root == pos) {
                static Block<keyType, valueType> newroot;
                newroot.isLeaf = false;
                newroot.siz = 1;
                newroot.ele[0] = cur.ele[MinSize];
                newroot.chd[0] = pos;
                newroot.chd[1] = newpos;
                wtfirst(pos, cur);
                wtfirst(newpos, blk);
                int rootpos = GetPos();
                wtall(rootpos, newroot);
                root = rootpos;
                return false;
            }
            wtall(pos, cur);
            wtall(newpos, blk);
            pass = blk.ele[0];
            return true; // 调整
        }

        // 不是叶子
        int l = 0, r = cur.siz;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (cur.ele[mid] >= ele) {
                r = mid;
            }
            else {
                l = mid + 1;
            }
        }
        if (l < cur.siz && cur.ele[l] == ele) {
            ++l;
        }
        // 就是 l
        Block<keyType, valueType> child;
        rdall(cur.chd[l], child);
        bool state = InternalInsert(child, cur.chd[l], ele);
        if (!state) return false;

        if (cur.siz < MaxSize) {
            for (int i = cur.siz - 1; i >= l; --i) {
                cur.ele[i + 1] = cur.ele[i];
                cur.chd[i + 2] = cur.chd[i + 1];
            }
            ++cur.siz;
            cur.ele[l] = pass;
            cur.chd[l + 1] = LastPos();
            wtall(pos, cur);
            return false;
        }
        // 继续裂块
        for (int i = cur.siz - 1; i >= l; --i) {
            cur.ele[i + 1] = cur.ele[i];
            cur.chd[i + 2] = cur.chd[i + 1];
        }
        ++cur.siz;
        cur.ele[l] = pass;
        cur.chd[l + 1] = LastPos();
        int newpos = GetPos();
        pass = cur.ele[MinSize];
        static Block<keyType, valueType> blk;
        blk.isLeaf = false;
        blk.siz = MinSize;
        for (int i = 0; i < MinSize; ++i) {
            blk.ele[i] = cur.ele[i + MinSize + 1];
            blk.chd[i] = cur.chd[i + MinSize + 1];
        }
        blk.chd[MinSize] = cur.chd[cur.siz];
        cur.siz = MinSize;
        if (root == pos) {
            // 裂根
            static Block<keyType, valueType> newroot;
            newroot.isLeaf = false;
            newroot.siz = 1;
            newroot.ele[0] = pass;
            newroot.chd[0] = pos;
            newroot.chd[1] = newpos;
            wtall(pos, cur);
            wtall(newpos, blk);
            int rootpos = GetPos();
            wtall(rootpos, newroot);
            root = rootpos;
            return false;
        }
        wtall(pos, cur);
        wtall(newpos, blk);
        return true;
    }

    bool InternalDelete(Block<keyType, valueType> &cur, int pos, const Element<keyType, valueType> &ele) {
        if (cur.isLeaf) {
            int l = 0, r = cur.siz;
            while (l < r) {
                int mid = (l + r) >> 1;
                if (cur.ele[mid] > ele) {
                    r = mid;
                }
                else {
                    l = mid + 1;
                }
            }
            --l;
            if (l < 0 || l >= cur.siz || cur.ele[l] != ele) {
                return false;
            }
            for (int i = l + 1; i < cur.siz; ++i) {
                cur.ele[i - 1] = cur.ele[i];
            }
            --cur.siz;
            if (pos == root) {
                wtall(pos, cur);
            }
            wtfirst(pos, cur);
            if (cur.siz < MinSize) {
                return true; // 并块
            }
            // 不用操作
            return false;
        }

        // 不是叶子
        int l = 0, r = cur.siz;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (cur.ele[mid] >= ele) {
                r = mid;
            }
            else {
                l = mid + 1;
            }
        }
        if (l < cur.siz && ele == cur.ele[l]) {
            // 刚好碰到，证明是右边的
            ++l;
        }
        // 就是 l
        Block<keyType, valueType> child;
        rdall(cur.chd[l], child);
        bool state = InternalDelete(child, cur.chd[l], ele);
        if (!state) return false;

        // 并块！此时 child 已经删掉了一个元素，考虑跟相邻两个元素之一合并
        // 合并
        // 特判根！如果根的孩子要并块且并完只剩一个块，那么这个根消灭
        if (pos == root && cur.siz == 1) {
            static Block<keyType, valueType> blk[2];
            rdsize(cur.chd[0], blk[0]);
            rdsize(cur.chd[1], blk[1]);
            if (blk[0].siz + blk[1].siz < MaxSize) {
                // 并块！
                rdall(cur.chd[0], blk[0]);
                rdall(cur.chd[1], blk[1]);
                Recycle(cur.chd[1]);
                Recycle(root);
                if (blk[0].isLeaf) {
                    for (int i = 0; i < blk[1].siz; ++i) {
                        blk[0].ele[i + blk[0].siz] = blk[1].ele[i];
                    }
                    blk[0].siz += blk[1].siz;
                    blk[0].nxt = blk[1].nxt;
                    root = cur.chd[0];
                    wtfirst(cur.chd[0], blk[0]);
                    return false;
                }
                for (int i = 0; i < blk[1].siz; ++i) {
                    blk[0].ele[i + blk[0].siz + 1] = blk[1].ele[i];
                    blk[0].chd[i + blk[0].siz + 1] = blk[1].chd[i];
                }
                blk[0].chd[blk[0].siz + blk[1].siz + 1] = blk[1].chd[blk[1].siz];
                blk[0].ele[blk[0].siz] = cur.ele[0];
                blk[0].siz += blk[1].siz + 1;
                root = cur.chd[0];
                wtall(cur.chd[0], blk[0]);
                return false;
            }
        }
        if (l > 0) {
            // 考虑和左边借元素 / 合并
            static Block<keyType, valueType> blk;
            rdsize(cur.chd[l - 1], blk);
            if (blk.siz > MinSize) {
                // 从左边借一个
                if (child.isLeaf) {
                    rdfirst(cur.chd[l - 1], blk);
                    for (int i = child.siz - 1; i >= 0; --i) {
                        child.ele[i + 1] = child.ele[i];
                    }
                    child.ele[0] = blk.ele[blk.siz - 1];
                    ++child.siz;
                    --blk.siz;
                    cur.ele[l - 1] = child.ele[0];
                    wtall(pos, cur);
                    wtsize(cur.chd[l - 1], blk);
                    wtfirst(cur.chd[l], child);
                    return false;
                }
                rdall(cur.chd[l - 1], blk);
                for (int i = child.siz; i >= 1; --i) {
                    child.ele[i] = child.ele[i - 1];
                    child.chd[i + 1] = child.chd[i];
                }
                child.chd[1] = child.chd[0];
                ++child.siz;
                child.ele[0] = cur.ele[l - 1];
                child.chd[0] = blk.chd[blk.siz];
                cur.ele[l - 1] = blk.ele[blk.siz - 1];
                --blk.siz;
                wtall(pos, cur);
                wtsize(cur.chd[l - 1], blk);
                wtall(cur.chd[l], child);
                return false;
            }
            // 和左边合并
            if (child.isLeaf) {
                rdfirst(cur.chd[l - 1], blk);
                Recycle(cur.chd[l]);
                for (int i = 0; i < child.siz; ++i) {
                    blk.ele[i + blk.siz] = child.ele[i];
                }
                blk.siz += child.siz;
                blk.nxt = child.nxt;
                for (int i = l; i < cur.siz; ++i) {
                    cur.ele[i - 1] = cur.ele[i];
                    cur.chd[i] = cur.chd[i + 1];
                }
                --cur.siz;
                blk.nxt = child.nxt;
                wtall(pos, cur);
                wtfirst(cur.chd[l - 1], blk);
                if (cur.siz < MinSize) return true;
                return false;
            }
            rdall(cur.chd[l - 1], blk);
            Recycle(cur.chd[l]);
            for (int i = 0; i < child.siz; ++i) {
                blk.ele[i + blk.siz + 1] = child.ele[i];
                blk.chd[i + blk.siz + 1] = child.chd[i];
            }
            blk.chd[blk.siz + child.siz + 1] = child.chd[child.siz];
            blk.ele[blk.siz] = cur.ele[l - 1];
            blk.siz += child.siz + 1;
            for (int i = l - 1; i < cur.siz - 1; ++i) {
                cur.ele[i] = cur.ele[i + 1];
                cur.chd[i + 1] = cur.chd[i + 2];
            }
            --cur.siz;
            wtall(pos, cur);
            wtall(cur.chd[l - 1], blk);
            if (cur.siz < MinSize) return true;
            return false;
        }
        else if (l < cur.siz) {
            // 和右边借元素 / 合并
            static Block<keyType, valueType> blk;
            rdsize(cur.chd[l + 1], blk);
            if (blk.siz > MinSize) {
                // 从右边借一个
                if (child.isLeaf) {
                    rdfirst(cur.chd[l + 1], blk);
                    child.ele[child.siz] = blk.ele[0];
                    ++child.siz;
                    for (int i = 0; i < blk.siz - 1; ++i) {
                        blk.ele[i] = blk.ele[i + 1];
                    }
                    --blk.siz;
                    cur.ele[l] = blk.ele[0];
                    wtall(pos, cur);
                    wtfirst(cur.chd[l], child);
                    wtfirst(cur.chd[l + 1], blk);
                    return false;
                }
                rdall(cur.chd[l + 1], blk);
                child.ele[child.siz] = cur.ele[l];
                child.chd[child.siz + 1] = blk.chd[0];
                ++child.siz;
                cur.ele[l] = blk.ele[0];
                for (int i = 0; i < blk.siz - 1; ++i) {
                    blk.ele[i] = blk.ele[i + 1];
                    blk.chd[i] = blk.chd[i + 1];
                }
                blk.chd[blk.siz - 1] = blk.chd[blk.siz];
                --blk.siz;
                wtall(pos, cur);
                wtall(cur.chd[l], child);
                wtall(cur.chd[l + 1], blk);
                return false;
            }
            // 和右边合并
            if (child.isLeaf) {
                rdfirst(cur.chd[l + 1], blk);
                Recycle(cur.chd[l + 1]);
                for (int i = 0; i < blk.siz; ++i) {
                    child.ele[i + child.siz] = blk.ele[i];
                }
                child.siz += blk.siz;
                child.nxt = blk.nxt;
                for (int i = l; i < cur.siz - 1; ++i) {
                    cur.ele[i] = cur.ele[i + 1];
                    cur.chd[i + 1] = cur.chd[i + 2];
                }
                --cur.siz;
                child.nxt = blk.nxt;
                wtall(pos, cur);
                wtfirst(cur.chd[l], child);
                if (cur.siz < MinSize) return true;
                return false;
            }
            rdall(cur.chd[l + 1], blk);
            Recycle(cur.chd[l + 1]);
            for (int i = 0; i < blk.siz; ++i) {
                child.ele[i + child.siz + 1] = blk.ele[i];
                child.chd[i + child.siz + 1] = blk.chd[i];
            }
            child.chd[child.siz + blk.siz + 1] = blk.chd[blk.siz];
            child.ele[child.siz] = cur.ele[l];
            child.siz += blk.siz + 1;
            for (int i = l; i < cur.siz - 1; ++i) {
                cur.ele[i] = cur.ele[i + 1];
                cur.chd[i + 1] = cur.chd[i + 2];
            }
            --cur.siz;
            wtall(pos, cur);
            wtall(cur.chd[l], child);
            if (cur.siz < MinSize) return true;
            return false;
        }
        else {
            exit(1);
        }
    }

public:
    int root = -1;
    BPlusTree() = default;
    explicit BPlusTree(const std::string &name) {
        _filename = name;
        _file.open(_filename);
        if (!_file) {
            // 文件不存在，创建新文件
            _file.open(_filename, std::fstream::out);
            _file.close();
            _file.open(_filename);
            nowsize = -1;
            _file.seekp(0);
            _file.write(reinterpret_cast<const char *>(&nowsize), sizeof(nowsize));
            root = -1;
            _file.write(reinterpret_cast<const char *>(&root), sizeof(root));
            recycle[0] = 0;
            _file.write(reinterpret_cast<const char *>(&recycle), sizeof(recycle));
            static Block<keyType, valueType> tmp;
            _file.write(reinterpret_cast<const char *>(&tmp), sizeof(tmp));
        }
        else {
            _file.seekg(0);
            _file.read(reinterpret_cast<char *>(&nowsize), sizeof(nowsize));
            _file.read(reinterpret_cast<char *>(&root), sizeof(root));
            _file.read(reinterpret_cast<char *>(&recycle), sizeof(recycle));
        }
    }
    ~BPlusTree() {
        _file.seekp(0);
        _file.write(reinterpret_cast<const char *>(&nowsize), sizeof(nowsize));
        _file.write(reinterpret_cast<const char *>(&root), sizeof(root));
        _file.write(reinterpret_cast<const char *>(&recycle), sizeof(recycle));
        _file.close();
    }

    void find(const keyType &key) {
        // 问题：头顶上是 51，结果右侧儿子的最小值居然是 50！
        if (root == -1) {
            cout << "null\n";
            return;
        }
        static Block<keyType, valueType> cur;
        cur.isLeaf = false;
        int pos = root;
        while (true) {
            rdall(pos, cur);
            if (cur.isLeaf) {
                break;
            }
            // 二分
            int l = 0, r = cur.siz;
            while (l < r) {
                int mid = (l + r) >> 1;
                if (cur.ele[mid].str >= key) {
                    r = mid;
                }
                else {
                    l = mid + 1;
                }
            }
            // 就是 l
            pos = cur.chd[l];
        }

        int l = 0, r = cur.siz;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (cur.ele[mid].str >= key) {
                r = mid;
            }
            else {
                l = mid + 1;
            }
        }
        if (l > 0) --l;
        // l 为第一个可能值
        if (l < cur.siz && key < cur.ele[l].str) {
            cout << "null\n";
            return;
        }
        bool flag = false, flag2 = false;
        while (true) {
            for (int i = l; i < cur.siz; ++i) {
                if (key < cur.ele[i].str) {
                    flag = true;
                    break;
                }
                if (key == cur.ele[i].str) {
                    flag2 = true;
                    cout << cur.ele[i].val << " ";
                }
            }
            if (flag) break;
            pos = cur.nxt;
            if (pos == -1) break;
            rdfirst(pos, cur);
            l = 0;
        }
        if (flag2) {
            cout << "\n";
        }
        else {
            cout << "null\n";
        }
    }

    void realInsert(const Element<keyType, valueType> &ele) {
        if (root == -1) {
            root = nowsize = 0;
            static Block<keyType, valueType> cur;
            cur.siz = 1;
            cur.ele[0] = ele;
            cur.isLeaf = true;
            cur.nxt = -1;
            wtfirst(nowsize, cur);
            return;
        }
        Block<keyType, valueType> cur;
        rdall(root, cur);
        InternalInsert(cur, root, ele);
    }

    void realDelete(const Element<keyType, valueType> &ele) {
        if (root == -1) return;
        static Block<keyType, valueType> cur;
        rdall(root, cur);
        InternalDelete(cur, root, ele);
    }

    void findall() {
        int pos = root;
        static Block<keyType, valueType> cur;
        cur.isLeaf = false;
        while (cur.isLeaf == false) {
            rdall(pos, cur);
            pos = cur.chd[0];
        }
        while (pos != -1) {
            std::cout << "new node, id = " << pos << std::endl;
            rdall(pos, cur);
            for (int i = 0; i < cur.siz; ++i) {
                std::cout << cur.ele[i].str << ' ' << cur.ele[i].val << std::endl;
            }
            pos = cur.nxt;
        }
        // 现在遍历整个树结构
        cout << "root = " << root << endl;
        cout << "nowsize = " << nowsize << endl;
        for (int i = 0; i <= nowsize; ++i) {
            cout << "point:" << i << endl;
            rdall(i, cur);
            cout << "siz" << cur.siz << endl;
            for (int j = 0; j < cur.siz; ++j) {
                cout << cur.ele[j].str << ' ' << cur.ele[j].val << endl;
            }
            if (cur.isLeaf) continue;
            for (int j = 0; j <= cur.siz; ++j) {
                cout << cur.chd[j] << ' ';
            }
            cout << endl;
        }
    }
};

#endif // BPlus_TREE
