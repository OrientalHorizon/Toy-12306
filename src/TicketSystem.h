//
// Created by iakno on 2023/5/26.
//

#ifndef TOY_12306_TICKETSYSTEM_H
#define TOY_12306_TICKETSYSTEM_H

#include "BPT.h"
#include "utils.h"
#include "UserSystem.h"
#include "TrainSystem.h"
#include "exceptions.h"
#include <algorithm>
#include <vector>

extern UserSystem userSystem;
extern TrainSystem trainSystem;

struct Order {
    int status = 0; // 1: 成功，2: 候补，-1: 退票
    int user_num, train_num, del_date;
    int stationID[2];
    int unit_price, num;
    MyID userID, trainID;
    Date realDate;
    MyString stationName[2];
    DateTime tim[2];
    Order() = default;
};

class TicketSystem {
private:
    int n;
    BPlusTree<std::pair<int, int>, int> queueIndex; // 候补订单：车次（indexed）+日期（相对） -> 订单编号
    BPlusTree<int, int> orderIndex; // 订单：用户 id -> 订单编号
    std::fstream _file;
    const std::string _filename = "ticket.dat";
public:
    TicketSystem() : orderIndex("OrderIndex"), queueIndex("QueueIndex") {
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
            static Order tmp;
            _file.write(reinterpret_cast<const char *>(&tmp), sizeof(tmp));
        }
        else {
            _file.seekg(0);
            _file.read(reinterpret_cast<char *>(&n), sizeof(n));
        }
    }
    ~TicketSystem() {
        _file.seekp(0);
        _file.write(reinterpret_cast<const char *>(&n), sizeof(n));
        _file.close();
    }
    bool BuyTicket(const std::string &username, const std::string &ID, const std::string &date, const std::string &from, const std::string &to, const int num, const bool que, const std::string &time_stamp) {
        MyID userID(username), trainID(ID);
        int user_num = userSystem.Logged(userID);
        if (user_num == -1) {
            cout << "-1" << endl;
            return false;
        }
        int train_num = trainSystem.Released(ID);
        if (train_num == -1) {
            cout << "-1" << endl;
            return false;
        }
        Train cur;
        trainSystem.Exists(trainID, cur);
        int stationID[2] = {-1, -1};
        for (int i = 0; i < cur.stationNum; ++i) {
            if (cur.stations[i] == from) stationID[0] = i;
            if (cur.stations[i] == to) stationID[1] = i;
        }
        if (stationID[0] == -1 || stationID[1] == -1 || stationID[0] >= stationID[1]) {
            cout << "-1" << endl;
            return false;
        }
        Date _date(date);
        // 检查始发日期
        int del_date = _date - cur.saleDate[0] - cur.departTime[stationID[0]].GetDay();
        if (del_date < 0 || del_date >= cur.saleDate[1] - cur.saleDate[0] + 1) {
            cout << "-1" << endl;
            return false;
        }
        // 查看有没有余票
        if (cur.totSeatNum < num) {
            cout << "-1" << endl;
            return false;
        }
        bool enough_ticket = true;
//        if (time_stamp == (std::string)"[126044]") {
//            cout << "fuck" << endl;
//        }
        for (int i = stationID[0]; i < stationID[1]; ++i) {
            // if (ID == "LeavesofGrass") {
                // cout << cur.seatNum[del_date][i] << endl;
                // cout << "num = " << num << endl;
            // }
            if (cur.seatNum[del_date][i] < num) {
                enough_ticket = false;
                break;
            }
        }
        if (!enough_ticket && !que) {
            // 没有余票，并且不愿意候补购票
            cout << "-1" << endl;
            return false;
        }
        static Order tmp;
        tmp.userID = userID;
        tmp.trainID = trainID;
        tmp.user_num = user_num;
        tmp.train_num = train_num;
        tmp.realDate = _date - cur.departTime[stationID[0]].GetDay();
        tmp.del_date = del_date;
        tmp.stationID[0] = stationID[0];
        tmp.stationID[1] = stationID[1];
        tmp.stationName[0] = cur.stations[stationID[0]];
        tmp.stationName[1] = cur.stations[stationID[1]];
        DateTime tmp1(tmp.realDate, cur.departTime[stationID[0]]), tmp2(tmp.realDate, cur.arriveTime[stationID[1]]);
        tmp.tim[0] = tmp1, tmp.tim[1] = tmp2;
        tmp.num = num;
        tmp.unit_price = cur.prices[stationID[1]] - cur.prices[stationID[0]];
        if (enough_ticket) {
            // 有余票
            for (int i = stationID[0]; i < stationID[1]; ++i) {
                cur.seatNum[del_date][i] -= num;
            }
            int tot = tmp.unit_price * num;
            trainSystem.Modify(train_num, cur);
            tmp.status = 1;
            orderIndex.realInsert(std::make_pair(user_num, ++n));
            _file.seekp(4 + n * sizeof(Order));
            _file.write(reinterpret_cast<const char *>(&tmp), sizeof(tmp));
            cout << tot << endl;
            return true;
        }
        // 没有余票，愿意候补购票
        // cout << "fuck" << endl;
        tmp.status = 2;
        ++n;
        queueIndex.realInsert(std::make_pair(std::make_pair(train_num, del_date), n));
        orderIndex.realInsert(std::make_pair(user_num, n));
        _file.seekp(4 + n * sizeof(Order));
        _file.write(reinterpret_cast<const char *>(&tmp), sizeof(tmp));
        cout << "queue" << endl;
        return true;
    }
    bool QueryOrder(const std::string &username) {
        MyID userID(username);
        int user_num = userSystem.Logged(userID);
        if (user_num == -1) {
            cout << "-1" << endl;
            return false;
        }
        sjtu::vector<int> orders = orderIndex.Find(user_num);
        if (orders.empty()) {
            cout << "0" << endl;
            return true;
        }
        cout << orders.size() << endl;
        static Order tmp;
        for (int i = orders.size() - 1; i >= 0; --i) {
            _file.seekg(4 + orders[i] * sizeof(Order));
            _file.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
            switch (tmp.status) {
                case 1: {
                    cout << "[success] ";
                    break;
                }
                case 2: {
                    cout << "[pending] ";
                    break;
                }
                case -1: {
                    cout << "[refunded] ";
                    break;
                }
                default: {
                    throw sjtu::exception("", "Invalid status.");
                    break;
                }
            }
            cout << tmp.trainID << " " << tmp.stationName[0] << " " << tmp.tim[0].ToString() << " -> " << tmp.stationName[1]
                 << " " << tmp.tim[1].ToString() << " " << tmp.unit_price << " " << tmp.num << endl;
        }
        return true;
    }
    bool RefundTicket(const std::string &username, const int _num, const std::string &time_stamp) {
        MyID userID(username);
        int user_num = userSystem.Logged(userID);
        if (user_num == -1) {
            cout << "-1" << endl;
            return false;
        }
        sjtu::vector<int> orders = orderIndex.Find(user_num);
        if ((int)orders.size() < _num) {
            cout << "-1" << endl;
            return false;
        }
        int real_num = orders.size() - _num;
        Order cur;
        _file.seekg(4 + orders[real_num] * sizeof(Order));
        _file.read(reinterpret_cast<char *>(&cur), sizeof(cur));
        if (cur.status == -1) {
            cout << "-1" << endl;
            return false;
        }
        if (cur.status == 2) {
            // 候补购票，不涉及具体车次的余票变化
            cur.status = -1;
            queueIndex.realDelete(std::make_pair(std::make_pair(cur.train_num, cur.del_date), orders[real_num]));
            _file.seekp(4 + orders[real_num] * sizeof(Order));
            _file.write(reinterpret_cast<const char *>(&cur.status), sizeof(cur.status));
            cout << "0" << endl;
            return true;
        }
        cur.status = -1;
        // 退票
        static Train cur_train;
        trainSystem.Query(cur.train_num, cur_train);
        for (int i = cur.stationID[0]; i < cur.stationID[1]; ++i) {
            cur_train.seatNum[cur.del_date][i] += cur.num;
        }
//        cout << cur_train.stationNum << endl;
//        for (int i = 0; i < 100; ++i) {
//            cout << cur_train.seatNum[cur.del_date][i] << " ";
//        }
//        cout << endl;
        // 遍历候补队列
        sjtu::vector<int> ques = queueIndex.Find(std::make_pair(cur.train_num, cur.del_date));
        Order tmp;
        for (size_t i = 0; i < ques.size(); ++i) {
            _file.seekg(4 + ques[i] * sizeof(Order));
            _file.read(reinterpret_cast<char *>(&tmp), 32);
            if (tmp.stationID[1] < cur.stationID[0] || tmp.stationID[0] > cur.stationID[1]) continue;
            bool enough_seats = true;
            for (int j = tmp.stationID[0]; j < tmp.stationID[1]; ++j) {
                if (cur_train.seatNum[cur.del_date][j] < tmp.num) {
                    enough_seats = false;
                    break;
                }
            }
            if (!enough_seats) continue;

            for (int j = tmp.stationID[0]; j < tmp.stationID[1]; ++j) {
                cur_train.seatNum[cur.del_date][j] -= tmp.num;
            }
            tmp.status = 1;
            _file.seekp(4 + ques[i] * sizeof(Order));
            _file.write(reinterpret_cast<const char *>(&tmp), sizeof(tmp.status));
            queueIndex.realDelete(std::make_pair(std::make_pair(tmp.train_num, tmp.del_date), ques[i]));
        }
//        for (int i = 0; i < 100; ++i) {
//            cout << cur_train.seatNum[cur.del_date][i] << " ";
//        }
//        cout << endl;
        trainSystem.Modify(cur.train_num, cur_train);
        _file.seekp(4 + orders[real_num] * sizeof(Order));
        _file.write(reinterpret_cast<char *>(&cur.status), sizeof(cur.status));
        cout << "0" << endl;
        return true;
    }
    bool QueryTicket(const std::string &date, const std::string &from, const std::string &to, bool _type) {
        // _type: true, cost; false, time
        sjtu::vector<std::pair<int, int>> vec[2] = {trainSystem.stationIndex.Find(from),
                                                    trainSystem.stationIndex.Find(to)};
        // sjtu::vector<int> trainID;
        sjtu::vector<std::pair<int, int>> stationID;
        sjtu::vector<Train> trains;
        Date _date(date);
        size_t i = 0, j = 0;
        static Train tmp;
        while (i < vec[0].size() && j < vec[1].size()) {
            if (vec[0][i].first == vec[1][j].first) {
                if (vec[0][i].second < vec[1][j].second) {
                    int train_num = vec[0][i].first;
                    trainSystem.Query(train_num, tmp);
                    // 还要读进来到达时间才能判断当天发车的情况
                    Date l = tmp.saleDate[0] + tmp.departTime[vec[0][i].second].GetDay(), r = tmp.saleDate[1] + tmp.departTime[vec[0][i].second].GetDay();
                    if (_date < l || r < _date) {
                        ++i, ++j;
                        continue;
                    }
                    trains.push_back(tmp);
                    stationID.push_back(std::make_pair(vec[0][i].second, vec[1][j].second));
                }
                ++i;
                ++j;
            } else if (vec[0][i].first < vec[1][j].first) {
                ++i;
            } else {
                ++j;
            }
        }
        if (trains.empty()) {
            cout << "0" << endl;
            return false;
        }
        sjtu::vector<int> _tim;
        _tim.resize(trains.size());
        sjtu::vector<int> prices;
        prices.resize(trains.size());
        for (size_t i = 0; i < trains.size(); ++i) {
            const Train &tmp = trains[i];
            prices.push_back(tmp.prices[stationID[i].second] - tmp.prices[stationID[i].first]);
            _tim.push_back(tmp.arriveTime[stationID[i].second] - tmp.departTime[stationID[i].first]);
        }
        std::vector<std::pair<std::pair<int, MyID>, int>> tmpVec;
        tmpVec.resize(trains.size());
        for (size_t i = 0; i < trains.size(); ++i) {
            if (_type) {
                tmpVec.push_back(std::make_pair(std::make_pair(prices[i],trains[i].id), i));
            } else {
                tmpVec.push_back(std::make_pair(std::make_pair(_tim[i],trains[i].id), i));
            }
        }
//        if (_type) {
//            for (size_t i = 0; i < tmpVec.size(); ++i) {
//                cout << tmpVec[i].first.first << " " << tmpVec[i].first.second << " " << tmpVec[i].second << endl;
//            }
//        }
        std::sort(tmpVec.begin(), tmpVec.end());
        cout << tmpVec.size() << endl;
        for (size_t i = 0; i < tmpVec.size(); ++i) {
            int cur_id = tmpVec[i].second;
            Date realDate = _date - trains[cur_id].departTime[stationID[cur_id].first].GetDay();
            int del_date = realDate - trains[cur_id].saleDate[0];
            int max_ticket = 2147483647;
            for (size_t j = stationID[cur_id].first; j < stationID[cur_id].second; ++j) {
                max_ticket = std::min(max_ticket, trains[cur_id].seatNum[del_date][j]);
            }
            DateTime departTime(realDate, trains[cur_id].departTime[stationID[cur_id].first]);
            DateTime arriveTime(realDate, trains[cur_id].arriveTime[stationID[cur_id].second]);
            cout << trains[cur_id].id << " " << from << " " << departTime.ToString() <<  " -> " << to << " " << arriveTime.ToString() << " " << prices[cur_id] << " " << max_ticket << endl;
        }
        return true;
    }
    bool Compare(int price, int _tim, const MyID &id_1, const MyID &id_2, int cur_price, int cur_tim, const MyID &cur_id_1, const MyID &cur_id_2, bool _type) {
        if (_type) {
            // cost
            if (price != cur_price) {
                return cur_price < price;
            }
            if (_tim != cur_tim) {
                return cur_tim < _tim;
            }
            if (!(id_1 == cur_id_1)) {
                return cur_id_1 < id_1;
            }
            return cur_id_2 < id_2;
        }
        else {
            // time
            if (_tim != cur_tim) {
                return cur_tim < _tim;
            }
            if (price != cur_price) {
                return cur_price < price;
            }
            if (!(id_1 == cur_id_1)) {
                return cur_id_1 < id_1;
            }
            return cur_id_2 < id_2;
        }
    }
    bool QueryTransfer(const std::string &date, const std::string &from, const std::string &to, bool _type, const std::string &time_stamp) {
        // _type: true, cost; false, time
        // 方式：from 往后扫，to 往前扫；找到相同的车站，然后判断时间是否合适
        Date _date(date);
        sjtu::vector<std::pair<int, int>> stationsFrom, stationsTo;
        sjtu::vector<std::pair<int, int>> stationIDFrom, stationIDTo;
        sjtu::vector<std::pair<int, int>> vec[2] = {trainSystem.stationIndex.Find(from),
                                                    trainSystem.stationIndex.Find(to)};
        int price = 2147483646, _tim = 2147483646;
        MyID id_1("~"), id_2("~");
        int ans[2] = {-1, -1}; // 前后车次
        std::pair<int, int> stationID[2];
        Date realDate[2];

        // cmp_ans: 比较用，ans: 最优解
        static Train cur_1;
        static Train cur_2;
        for (size_t i = 0; i < vec[0].size(); ++i) {
            trainSystem.Query(vec[0][i].first, cur_1);
            // 在那天有车
            Date l = cur_1.saleDate[0] + cur_1.departTime[vec[0][i].second].GetDay(), r = cur_1.saleDate[1] + cur_1.departTime[vec[0][i].second].GetDay();
            if (_date < l || r < _date) {
                continue;
            }
            for (size_t j = 0; j < vec[1].size(); ++j) {
                if (vec[0][i].first == vec[1][j].first) {
                    continue;
                }
                trainSystem.Query(vec[1][j].first, cur_2);

                // Check common stations
                for (size_t k1 = vec[0][i].second + 1; k1 < cur_1.stationNum; ++k1) {
                    for (size_t k2 = 0; k2 < vec[1][j].second; ++k2) {
                        if (!(cur_1.stations[k1] == cur_2.stations[k2])) {
                            continue;
                        }
                        // now check whether time is available
//                        if (time_stamp == "[38948]") {
//                            cout << "fuck" << endl;
//                            cout << cur_1.stations[k1] << " " << cur_2.stations[k2] << endl;
//                        }
                        Date l1 = _date + (cur_1.arriveTime[k1].GetDay() - cur_1.departTime[vec[0][i].second].GetDay());
                        Time l2 = cur_1.arriveTime[k1].EraseDay();
                        DateTime l3(l1, l2); // 第二趟车必须不早于这个时间发车
                        DateTime r3(cur_2.saleDate[1], cur_2.departTime[k2]);
                        if (r3 < l3) {
                            continue;
                        }
                        int cur_tim, cur_price;
                        DateTime l4(cur_2.saleDate[0], cur_2.departTime[k2]);
                        Date fuckRealDate;
                        if (l4 <= l3) {
                            // 可以直接看第一趟车到达时间发车
                            // 要判断到的那天能不能立马走
                            Time tmp1 = cur_1.arriveTime[k1].EraseDay();
                            Time tmp2 = cur_2.departTime[k2].EraseDay();
                            if (tmp1 <= tmp2) {
                                // 当天走
                                cur_tim = (cur_1.arriveTime[k1] - cur_1.departTime[vec[0][i].second]) + (tmp2 - tmp1) + (cur_2.arriveTime[vec[1][j].second] - cur_2.departTime[k2]);
                                fuckRealDate = Date(l3.GetDate().first, l3.GetDate().second) - cur_2.departTime[k2].GetDay();
                            }
                            else {
                                // 第二天走
                                cur_tim = (cur_1.arriveTime[k1] - cur_1.departTime[vec[0][i].second]) + (tmp2 - tmp1) + (cur_2.arriveTime[vec[1][j].second] - cur_2.departTime[k2]) + 1440;
                                fuckRealDate = Date(l3.GetDate().first, l3.GetDate().second) - cur_2.departTime[k2].GetDay() + 1;
                            }
                        }
                        else {
                            // 坐第二趟车首班
                            cur_tim = (cur_1.arriveTime[k1] - cur_1.departTime[vec[0][i].second]) + (cur_2.arriveTime[vec[1][j].second] - cur_2.departTime[k2]) + (l4 - l3);
                            fuckRealDate = cur_2.saleDate[0];
                        }
                        cur_price = cur_1.prices[k1] - cur_1.prices[vec[0][i].second] + cur_2.prices[vec[1][j].second] - cur_2.prices[k2];
                        bool ret = Compare(price, _tim, id_1, id_2, cur_price, cur_tim, cur_1.id, cur_2.id, _type);
                        if (ret) {
                            price = cur_price;
                            _tim = cur_tim;
                            id_1 = cur_1.id;
                            id_2 = cur_2.id;
                            ans[0] = vec[0][i].first, ans[1] = vec[1][j].first;
                            stationID[0] = std::make_pair(vec[0][i].second, k1);
                            stationID[1] = std::make_pair(k2, vec[1][j].second);
                            realDate[0] = _date - cur_1.departTime[vec[0][i].second].GetDay();
                            realDate[1] = fuckRealDate;
                        }
                    }
                }
            }
        }
        if (price == 2147483646) {
            cout << "0" << endl;
            return false;
        }

        // TODO: Output
        static Train cur;
        for (int j = 0; j < 2; ++j) {
            trainSystem.Query(ans[j], cur);
            int total_price = cur.prices[stationID[j].second] - cur.prices[stationID[j].first], max_seat = 2147483646;
            int del_date = realDate[j] - cur.saleDate[0];
            for (size_t i = stationID[j].first; i < stationID[j].second; ++i) {
                max_seat = std::min(max_seat, cur.seatNum[del_date][i]);
            }
            DateTime departTime(realDate[j], cur.departTime[stationID[j].first]);
            DateTime arriveTime(realDate[j], cur.arriveTime[stationID[j].second]);
            cout << cur.id << " " << cur.stations[stationID[j].first] << " " << departTime.ToString() << " -> " << cur.stations[stationID[j].second] << " " << arriveTime.ToString()
                 << " " << total_price << " " << max_seat << endl;
        }
        return true;
    }
    bool clear() {
        n = 0;
        queueIndex.clear();
        orderIndex.clear();
        return true;
    }
};



#endif //TOY_12306_TICKETSYSTEM_H
