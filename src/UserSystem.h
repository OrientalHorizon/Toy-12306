//
// Created by iakno on 2023/5/23.
//

#ifndef TOY_12306_USERSYSTEM_H
#define TOY_12306_USERSYSTEM_H

#include "BPT.h"
#include "utils.h"

struct User {
    MyID id;
    MyString password, name, mailAddr;
    int priv; // [0, 10]
    User() = default;
    User(const MyID &_id, const MyString &_password, const MyString &_name, const MyString &_mailAddr, int _priv) :
        id(_id), password(password), name(name), mailAddr(mailAddr), priv(priv) {}
};

class UserSystem {
    friend class TrainSystem;
    friend class TicketSystem;
private:
    int n; // number of users
    BPlusTree<MyID, int> userIndex;
    std::fstream _file;
    const std::string _filename = "user.dat";
    BPlusTree<MyID, int> loginStack;

public:
    UserSystem() : userIndex("UserIndex") {
        _file.open(_filename);
        if (!_file) {
            // 创建一个文件
            _file.open(_filename, std::fstream::out);
            _file.close();
            _file.open(_filename);
            n = -1;
            _file.seekp(0);
            _file.write(reinterpret_cast<const char *>(&n), sizeof(n));
            _file.seekp(4);
            static User tmp;
            _file.write(reinterpret_cast<const char *>(&tmp), sizeof(tmp));
        }
        else {
            _file.seekg(0);
            _file.read(reinterpret_cast<char *>(&n), sizeof(n));
        }
        loginStack.clear();
    }
    ~UserSystem() {
        _file.seekp(0);
        _file.write(reinterpret_cast<const char *>(&n), sizeof(n));
        _file.close();
        loginStack.clear();
    }
    int Exists(const MyID &id, User &cur) {
        sjtu::vector<int> ret = userIndex.Find(id);
        if (ret.empty()) return -1;
        if (ret.size() > 1) throw sjtu::exception("exception", "UserSystem::Exists: ret.size() > 1");
        _file.seekg(4 + ret[0] * sizeof(User));
        _file.read(reinterpret_cast<char *>(&cur), sizeof(cur));
        return ret[0];
    }
    int Logged(const MyID &id) {
        sjtu::vector<int> vec = loginStack.Find(id);
        if (vec.empty()) return -1;
        if (vec.size() > 1) throw sjtu::exception("exception", "UserSystem::Logged: vec.size() > 1");
        return vec[0];
    }
    bool Login(const std::string &ID, const std::string &pwd) {
        MyID cur_id(ID);
        static User cur;
        int tmp_int = Logged(cur_id);
        if (tmp_int != -1) {
            cout << "-1" << endl;
            return false;
        }
        int tmp_bool = Exists(cur_id, cur);
        if (tmp_bool == -1) {
            cout << "-1" << endl;
            return false;
        }
        if (cur.password == pwd) {
            cout << "0" << endl;
            loginStack.realInsert(std::make_pair(cur_id, tmp_bool));
            return true;
        }
        else {
            cout << "-1" << endl;
            return false;
        }
    }
    bool Logout(const std::string &ID) {
        MyID cur_id(ID);
        int tmp_int = Logged(cur_id);
        if (tmp_int == -1) {
            cout << "-1" << endl;
            return false;
        }
        static User cur;
        if (Exists(cur_id, cur) == -1) {
            cout << "-1" << endl;
            return false;
        }
        loginStack.realDelete(std::make_pair(cur_id, tmp_int));
        cout << "0" << endl;
        return true;
    }
    bool QueryProfile(const std::string &curID, const std::string &ID) {
        MyID curuser_id(curID), cur_id(ID);
        static User cur_user, cur;
        if (Exists(curuser_id, cur_user) == -1) {
            cout << "-1" << endl;
            return false;
        }
        if (Exists(cur_id, cur) == -1) {
            cout << "-1" << endl;
            return false;
        }
        if (Logged(curuser_id) == -1) {
            cout << "-1" << endl;
            return false;
        }
        if (cur_user.priv <= cur.priv && curID != ID) {
            cout << "-1" << endl;
            return false;
        }
        cout << cur.id << " " << cur.name << " " << cur.mailAddr << " " << cur.priv << endl;
        return true;
    }
    bool ModifyProfile(const std::string &curID, const std::string &ID, const std::string &pwd, const std::string &name, const std::string &mailAddr, const int priv) {
        MyID curuser_id(curID), cur_id(ID);
        static User cur_user, cur;
        if (Exists(curuser_id, cur_user) == -1) {
            cout << "-1" << endl;
            return false;
        }
        int pos = Exists(cur_id, cur);
        if (pos == -1) {
            cout << "-1" << endl;
            return false;
        }
        if (Logged(curuser_id) == -1) {
            cout << "-1" << endl;
            return false;
        }
        if (cur_user.priv <= cur.priv && curID != ID) {
            cout << "-1" << endl;
            return false;
        }
        if (cur_user.priv <= priv) {
            cout << "-1" << endl;
            return false;
        }
        if (pwd != "") cur.password = pwd;
        if (name != "") cur.name = name;
        if (mailAddr != "") cur.mailAddr = mailAddr;
        if (priv != -1) cur.priv = priv;
        _file.seekp(4 + pos * sizeof(User));
        _file.write(reinterpret_cast<const char *>(&cur), sizeof(cur));
        cout << cur.id << " " << cur.name << " " << cur.mailAddr << " " << cur.priv << endl;
        return true;
    }
    bool AddUser(const std::string &curID, const std::string &ID, const std::string &pwd, const std::string &name, const std::string &mailAddr, const int priv, const std::string &time_stamp) {
        MyID cur_id(ID);
        if (n == -1) {
            User cur;
            cur.id = cur_id;
            cur.password = pwd;
            cur.name = name;
            cur.mailAddr = mailAddr;
            cur.priv = 10;
            userIndex.realInsert(std::make_pair(cur_id, ++n));
            _file.seekp(4 + n * sizeof(User));
            _file.write(reinterpret_cast<const char *>(&cur), sizeof(cur));
            cout << "0" << endl;
            return true;
        }
        // cout << time_stamp << endl;
//        if (time_stamp == "[6728]") {
//            cout << curID << endl;
//        }
        if (priv == -1 || curID == "") {
            cout << "-1" << endl;
            return false;
        }
        MyID curuser_id(curID);
        static User cur_user, cur;
        if (Exists(curuser_id, cur_user) == -1) {
            // cout << "?" << endl;
            cout << "-1" << endl;
            return false;
        }
        if (Logged(curuser_id) == -1) {
            // cout << "??" << endl;
            cout << "-1" << endl;
            return false;
        }
        if (cur_user.priv <= priv) {
            // cout << "???" << endl;
            cout << "-1" << endl;
            return false;
        }
        if (Exists(cur_id, cur) != -1) {
            // cout << "????" << endl;
            cout << "-1" << endl;
            return false;
        }
        cur.id = cur_id;
        cur.password = pwd;
        cur.name = name;
        cur.mailAddr = mailAddr;
        cur.priv = priv;
        userIndex.realInsert(std::make_pair(cur_id, ++n));
        _file.seekp(4 + n * sizeof(User));
        _file.write(reinterpret_cast<const char *>(&cur), sizeof(cur));
        cout << "0" << endl;
        return true;
    }
    bool clear() {
        n = 0;
        loginStack.clear();
        userIndex.clear();
        return true;
    }
};


#endif //TOY_12306_USERSYSTEM_H
