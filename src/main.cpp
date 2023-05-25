//
// Created by iakno on 2023/5/23.
//
#include <utils.h>
#include "UserSystem.h"
#include "TokenScanner.h"
#include "BPT.h"
#include "TrainSystem.h"

using std::cin;
using std::cout;
using std::endl;

UserSystem userSystem;
TrainSystem trainSystem;

signed main() {
//    freopen("1.in", "r", stdin);
//    freopen("1-1.out", "w", stdout);
    std::string line;
    std::string cmd;
    bool is_first = true;
    while (getline(std::cin, line)) {
        std::stringstream ss;
        ss.clear();
        ss << line;
//        cout << "6" << ss.str() << endl;
        ss >> cmd;
        // Time stamp
        cout << cmd << " ";
        ss >> cmd;
        if (cmd == "exit") {
            cout << "bye" << endl;
            break;
        }
        else if (cmd == "clear") {
            // TODO: clear
            // call all systems to clear data
            userSystem.clear();
        }
        else if (cmd == "add_user") {
            std::string tmp, cur_username, username, password, name, mailAddr;
            int privilege;
            if (is_first) {
                for (int unused = 0; unused < 6; ++unused) {
                    ss >> tmp;
                    switch (tmp[1]) {
                        case 'u': {
                            ss >> username;
                            break;
                        }
                        case 'p': {
                            ss >> password;
                            break;
                        }
                        case 'n': {
                            ss >> name;
                            break;
                        }
                        case 'm': {
                            ss >> mailAddr;
                            break;
                        }
                        default: {
                            ss >> tmp;
                            break;
                        }
                    }
                }
                privilege = 10;
                userSystem.AddUser("", username, password, name, mailAddr, privilege);
                is_first = false;
                continue;
            }
            for (int unused = 0; unused < 6; ++unused) {
                ss >> tmp;
                switch (tmp[1]) {
                    case 'c': {
                        ss >> cur_username;
                        break;
                    }
                    case 'u': {
                        ss >> username;
                        break;
                    }
                    case 'p': {
                        ss >> password;
                        break;
                    }
                    case 'n': {
                        ss >> name;
                        break;
                    }
                    case 'm': {
                        ss >> mailAddr;
                        break;
                    }
                    case 'g': {
                        ss >> privilege;
                        break;
                    }
                }
            }
            userSystem.AddUser(cur_username, username, password, name, mailAddr, privilege);
        }
        else if (cmd == "login") {
            std::string tmp, username, password;
            for (int unused = 0; unused < 2; ++unused) {
                ss >> tmp;
                switch (tmp[1]) {
                    case 'u': {
                        ss >> username;
                        break;
                    }
                    case 'p': {
                        ss >> password;
                        break;
                    }
                }
            }
            userSystem.Login(username, password);
        }
        else if (cmd == "logout") {
            std::string tmp, username;
            ss >> tmp >> username;
            userSystem.Logout(username);
        }
        else if (cmd == "query_profile") {
            std::string tmp, cur_username, username;
            for (int unused = 0; unused < 2; ++unused) {
                ss >> tmp;
                switch (tmp[1]) {
                    case 'c': {
                        ss >> cur_username;
                        break;
                    }
                    case 'u': {
                        ss >> username;
                        break;
                    }
                }
            }
            userSystem.QueryProfile(cur_username, username);
        }
        else if (cmd == "modify_profile") {
            std::string tmp, cur_username, username, password, name, mailAddr;
            int privilege = -1;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'c': {
                        ss >> cur_username;
                        break;
                    }
                    case 'u': {
                        ss >> username;
                        break;
                    }
                    case 'p': {
                        ss >> password;
                        break;
                    }
                    case 'n': {
                        ss >> name;
                        break;
                    }
                    case 'm': {
                        ss >> mailAddr;
                        break;
                    }
                    case 'g': {
                        ss >> privilege;
                        break;
                    }
                }
            }
            userSystem.ModifyProfile(cur_username, username, password, name, mailAddr, privilege);
        }
        else if (cmd == "add_train") {
            std::string tmp, trainID, stations, prices, startTime, travelTimes, stopoverTimes, saleDate;
            int stationNum, seatNum;
            char _type;
            for (int unused = 0; unused < 10; ++unused) {
                ss >> tmp;
                switch (tmp[1]) {
                    case 'i': {
                        ss >> trainID;
                        break;
                    }
                    case 'n': {
                        ss >> stationNum;
                        break;
                    }
                    case 'm': {
                        ss >> seatNum;
                        break;
                    }
                    case 's': {
                        ss >> stations;
                        break;
                    }
                    case 'p': {
                        ss >> prices;
                        break;
                    }
                    case 'x': {
                        ss >> startTime;
                        break;
                    }
                    case 't': {
                        ss >> travelTimes;
                        break;
                    }
                    case 'o': {
                        ss >> stopoverTimes;
                        break;
                    }
                    case 'd': {
                        ss >> saleDate;
                        break;
                    }
                    case 'y': {
                        ss >> _type;
                        break;
                    }
                }
            }
            trainSystem.AddTrain(trainID, stationNum, seatNum, stations, prices, startTime, travelTimes, stopoverTimes, saleDate, _type);
        }
        else if (cmd == "delete_train") {
            std::string tmp, trainID;
            ss >> tmp >> trainID;
            trainSystem.DeleteTrain(trainID);
        }
        else if (cmd == "release_train") {
            std::string tmp, trainID;
            ss >> tmp >> trainID;
            trainSystem.ReleaseTrain(trainID);
        }
        else if (cmd == "query_train") {
            std::string tmp, trainID, date;
            for (int unused = 0; unused < 2; ++unused) {
                cin >> tmp;
                switch (tmp[1]) {
                    case 'i': {
                        cin >> trainID;
                        break;
                    }
                    case 'd': {
                        cin >> date;
                        break;
                    }
                }
            }
            trainSystem.QueryTrain(trainID, date);
        }
//        else if (cmd == "query_ticket") {
//            std::string tmp, date, from, to, _type;
//            ss >> tmp >> date >> from >> to >> _type;
//            trainSystem.QueryTicket(date, from, to, _type);
//        }
//        else if (cmd == "query_transfer") {
//            std::string tmp, date, from, to;
//            ss >> tmp >> date >> from >> to;
//            trainSystem.QueryTransfer(date, from, to);
//        }
//        else if (cmd == "buy_ticket") {
//            std::string tmp, cur_username, trainID, date, from, to;
//            int num;
//            char _type;
//            for (int unused = 0; unused < 6; ++unused) {
//                ss >> tmp;
//                switch (tmp[1]) {
//                    case 'c': {
//                        ss >> cur_username;
//                        break;
//                    }
//                    case 'i': {
//                        ss >> trainID;
//                        break;
//                    }
//                    case 'd': {
//                        ss >> date;
//                        break;
//                    }
//                    case 'n': {
//                        ss >> num;
//                        break;
//                    }
//                    case 'f': {
//                        ss >> from;
//                        break;
//                    }
//                    case 't': {
//                        ss >> to;
//                        break;
//                    }
//                    case 'y': {
//                        ss >> _type;
//                        break;
//                    }
//                }
//            }
//            trainSystem.BuyTicket(cur_username, trainID, date, num, from, to, _type);
//        }
//        else if (cmd == "query_order") {
//            std::string tmp, cur_username;
//            ss >> tmp >> cur_username;
//            trainSystem.QueryOrder(cur_username);
//        }
//        else if (cmd == "refund_ticket") {
//            std::string tmp, cur_username;
//            int num;
//            ss >> tmp >> cur_username >> num;
//            trainSystem.RefundTicket(cur_username, num);
//        }
    }
    return 0;
}