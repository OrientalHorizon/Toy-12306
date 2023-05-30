//
// Created by iakno on 2023/5/25.
//

#ifndef TOY_12306_TRAINSYSTEM_H
#define TOY_12306_TRAINSYSTEM_H

#include "BPT.h"
#include "utils.h"
#include "Calendar.h"

struct Train {
    MyID id;
    char released = 0;
    Date saleDate[2];
    int stationNum, totSeatNum, seatNum[94][100], prices[100]; // prices 改成前缀和
    MyString stations[100];
    Time departTime[100], arriveTime[100];
    char type; // 'G' 'D' 'Z' 'T' 'K' 'C'
    Train() = default;
};

class TrainSystem {
    friend class TicketSystem;
private:
    int n; // number of trains
    BPlusTree<MyID, int> trainIndex;
    std::fstream _file;
    const std::string _filename = "train.dat";
    BPlusTree<MyString, std::pair<int, int>> stationIndex;

public:
    TrainSystem() : trainIndex("TrainIndex"), stationIndex("StationIndex") {
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
            static Train tmp;
            _file.write(reinterpret_cast<const char *>(&tmp), sizeof(tmp));
        }
        else {
            _file.seekg(0);
            _file.read(reinterpret_cast<char *>(&n), sizeof(n));
        }
    }
    ~TrainSystem() {
        _file.seekp(0);
        _file.write(reinterpret_cast<const char *>(&n), sizeof(n));
        _file.close();
    }
    int Exists(const MyID &id, Train &cur) {
        sjtu::vector<int> ret = trainIndex.Find(id);
        if (ret.empty()) return -1;
        if (ret.size() > 1) throw sjtu::exception("exception", "TrainSystem::Exists: ret.size() > 1");
        _file.seekg(4 + ret[0] * sizeof(Train));
        _file.read(reinterpret_cast<char *>(&cur), sizeof(cur));
        return ret[0];
    }
    int Released(const MyID &id) {
        sjtu::vector<int> ret = trainIndex.Find(id);
        if (ret.empty()) return -1;
        if (ret.size() > 1) throw sjtu::exception("exception", "TrainSystem::Exists: ret.size() > 1");
        _file.seekg(4 + ret[0] * sizeof(Train) + sizeof(MyID));
        char tmp;
        _file.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
        if (!tmp) return -1;
        return ret[0];
    }
    void Query(int pos, Train &cur) {
        _file.seekg(4 + pos * sizeof(Train));
        _file.read(reinterpret_cast<char *>(&cur), sizeof(cur));
    }
    void Modify(int pos, Train &cur) {
        _file.seekp(4 + pos * sizeof(Train));
        _file.write(reinterpret_cast<const char *>(&cur), sizeof(cur));
    }
    bool AddTrain(const std::string &ID, const int stationNum, const int seatNum, const std::string &stations, const std::string &prices, const std::string &startTime,
                  const std::string &travelTimes, const std::string &stopoverTimes, const std::string &saleDate, const char _type) {
        MyID curID(ID);
        Train cur;
        if (Exists(curID, cur) != -1) { // id 已经被占了
            cout << "-1" << endl;
            return false;
        }
        cur.id = curID;
        cur.stationNum = stationNum;
        cur.type = _type;
        std::stringstream ss;
        ss << stations;
        for (int i = 0; i < stationNum; ++i) {
            std::string sta;
            getline(ss, sta, '|');
            cur.stations[i] = sta;
        }
        ss.clear();
        ss << prices;
        cur.prices[0] = 0;
        for (int i = 1; i < stationNum; ++i) {
            std::string pri;
            getline(ss, pri, '|');
            cur.prices[i] = std::stoi(pri) + cur.prices[i - 1];
        }
        ss.clear();
        Time st_tim(startTime);
        cur.departTime[0] = st_tim;

        int trav_tim[100];
        ss << travelTimes;
        for (int i = 0; i < stationNum - 1; ++i) {
            std::string tra;
            getline(ss, tra, '|');
            trav_tim[i] = std::stoi(tra);
        }
        ss.clear();

        int stop_tim[100];
        ss << stopoverTimes;
        if (stationNum > 2) {
            for (int i = 0; i < stationNum - 2; ++i) {
                std::string sto;
                getline(ss, sto, '|');
                stop_tim[i] = std::stoi(sto);
            }
        }
        ss.clear();
        for (int i = 1; i < stationNum - 1; ++i) {
            cur.arriveTime[i] = cur.departTime[i - 1] + trav_tim[i - 1];
            cur.departTime[i] = cur.arriveTime[i] + stop_tim[i - 1];
        }
        cur.arriveTime[stationNum - 1] = cur.departTime[stationNum - 2] + trav_tim[stationNum - 2];

        ss << saleDate;
        for (int i = 0; i < 2; ++i) {
            std::string dat;
            getline(ss, dat, '|');
            cur.saleDate[i] = Date(dat);
        }
        int sale_duration = cur.saleDate[1] - cur.saleDate[0] + 1;
        cur.totSeatNum = seatNum;
        for (int i = 0; i < sale_duration; ++i) {
            for (int j = 0; j < stationNum - 1; ++j) {
                cur.seatNum[i][j] = seatNum;
            }
        }
        trainIndex.realInsert(std::make_pair(curID, ++n));
        _file.seekp(4 + n * sizeof(Train));
        _file.write(reinterpret_cast<const char *>(&cur), sizeof(cur));
        cout << "0" << endl;
        return true;
    }
    bool DeleteTrain(const std::string &ID) {
        MyID curID(ID);
        Train cur;
        int tmp_int = Exists(curID, cur);
        if (tmp_int == -1) {
            cout << "-1" << endl;
            return false;
        }
        if (cur.released) {
            cout << "-1" << endl;
            return false;
        }
        trainIndex.realDelete(std::make_pair(curID, tmp_int));
        cout << "0" << endl;
        return true;
    }
    bool ReleaseTrain(const std::string &ID) {
        MyID curID(ID);
        Train cur;
        int tmp_int = Exists(curID, cur);
        if (tmp_int == -1 || cur.released) {
            cout << "-1" << endl;
            return false;
        }
        char released = 1;
        _file.seekp(4 + tmp_int * sizeof(Train) + sizeof(MyID));
        _file.write(reinterpret_cast<const char *>(&released), sizeof(released));
        for (int i = 0; i < cur.stationNum; ++i) {
            stationIndex.realInsert(std::make_pair(MyString(cur.stations[i]), std::make_pair(tmp_int, i)));
        }
        cout << "0" << endl;
        return true;
    }
    bool QueryTrain(const std::string &ID, const std::string &date) {
        MyID curID(ID);
        Train cur;
        int tmp_int = Exists(curID, cur);
        if (tmp_int == -1) {
            cout << "-1" << endl;
            return false;
        }
        Date cur_date = Date(date);
        int del_date = cur_date - cur.saleDate[0];
        if (del_date < 0 || del_date > cur.saleDate[1] - cur.saleDate[0]) {
            cout << "-1" << endl;
            return false;
        }
        cout << curID << " " << cur.type << endl;
        for (int i = 0; i < cur.stationNum; ++i) {
            cout << cur.stations[i] << " ";
            if (i == 0) {
                cout << "xx-xx xx:xx ";
            }
            else {
                Date tmp_date = cur_date + cur.arriveTime[i].GetDay();
                cout << tmp_date.ToString() << " ";
                cout << cur.arriveTime[i].ToString() << " ";
            }
            cout << "-> ";
            if (i == cur.stationNum - 1) {
                cout << "xx-xx xx:xx ";
            }
            else {
                Date tmp_date = cur_date + cur.departTime[i].GetDay();
                cout << tmp_date.ToString() << " ";
                cout << cur.departTime[i].ToString() << " ";
            }
            cout << cur.prices[i] << " ";
            if (i == cur.stationNum - 1) {
                cout << "x" << endl;
            }
            else {
                cout << cur.seatNum[del_date][i] << endl;
            }
        }
        return true;
    }

    bool clear() {
        n = 0;
        trainIndex.clear();
        return true;
    }

};


#endif //TOY_12306_TRAINSYSTEM_H
