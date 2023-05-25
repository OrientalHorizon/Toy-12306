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
    int stationNum, seatNum, prices[100];
    MyString stations[100], travelTimes[100], stopoverTimes[100];
    Time startTime;
    Date saleDate[2];
    char type;
    Train() = default;
};

class TrainSystem {
private:
    int n; // number of trains
    BPlusTree<MyID, int> trainIndex;
    std::fstream _file;
    const std::string _filename = "train.dat";

public:
    TrainSystem() : trainIndex("TrainIndex") {
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
        if (ret.empty()) return false;
        if (ret.size() > 1) throw sjtu::exception();
        _file.seekg(4 + ret[0] * sizeof(Train));
        _file.read(reinterpret_cast<char *>(&cur), sizeof(cur));
        return true;
    }
    bool AddTrain()

};


#endif //TOY_12306_TRAINSYSTEM_H
