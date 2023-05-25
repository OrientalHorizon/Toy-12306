//
// Created by iakno on 2023/5/25.
//

#ifndef TOY_12306_TRAINSYSTEM_H
#define TOY_12306_TRAINSYSTEM_H

#include "BPT.h"
#include "utils.h"

struct Train {
    MyID id;
    int stationNum, seatNum, prices[100];
    Train() = default;
    MyString stations[100];
    Train(const MyID &_id, const MyString &_name, int _stationNum, int _seatNum) :
        id(_id), name(_name), stationNum(_stationNum), seatNum(_seatNum) {}
};

#endif //TOY_12306_TRAINSYSTEM_H
