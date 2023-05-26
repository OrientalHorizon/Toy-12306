//
// Created by iakno on 2023/5/23.
//
#include <utils.h>
#include "UserSystem.h"
#include "TokenScanner.h"
#include "BPT.h"
#include "TrainSystem.h"
#include "TicketSystem.h"

using std::cin;
using std::cout;
using std::endl;

UserSystem userSystem;
TrainSystem trainSystem;
TicketSystem ticketSystem;
#ifndef DEBUG
// #define DEBUG
#endif
signed main() {
#ifdef DEBUG
    system("rm -rf user.dat");
    system("rm -rf train.dat");
    system("rm -rf ticket.dat");
    system("rm -rf UserIndex");
    system("rm -rf TrainIndex");
    system("rm -rf OrderIndex");
    system("rm -rf StationIndex");
    system("rm -rf QueueIndex");
    freopen("3-1.in", "r", stdin);
    freopen("3-2.out", "w", stdout);
#endif
    std::string line;
    std::string cmd;
    std::string time_stamp;
    bool is_first = true;
    while (getline(std::cin, line)) {
        std::stringstream ss;
        ss.clear();
        ss << line;
//        cout << "6" << ss.str() << endl;
        ss >> time_stamp;
        // Time stamp
        cout << time_stamp << " ";
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
                ss >> tmp;
                switch (tmp[1]) {
                    case 'i': {
                        ss >> trainID;
                        break;
                    }
                    case 'd': {
                        ss >> date;
                        break;
                    }
                }
            }
            trainSystem.QueryTrain(trainID, date);
        }
        else if (cmd == "query_ticket") {
            std::string tmp, date, from, to;
            bool _type = false; // time
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'd': {
                        ss >> date;
                        break;
                    }
                    case 's': {
                        ss >> from;
                        break;
                    }
                    case 't': {
                        ss >> to;
                        break;
                    }
                    case 'p': {
                        ss >> tmp;
                        if (tmp == "time") {
                            _type = false;
                        }
                        else if (tmp == "cost") {
                            _type = true;
                        }
                        else {
                            throw sjtu::exception("", "Invalid query type");
                        }
                        break;
                    }
                }
            }
            ticketSystem.QueryTicket(date, from, to, _type);
        }
        else if (cmd == "query_transfer") {
            std::string tmp, date, from, to;
            bool _type = false; // time
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'd': {
                        ss >> date;
                        break;
                    }
                    case 's': {
                        ss >> from;
                        break;
                    }
                    case 't': {
                        ss >> to;
                        break;
                    }
                    case 'p': {
                        ss >> tmp;
                        if (tmp == "time") {
                            _type = false;
                        }
                        else if (tmp == "cost") {
                            _type = true;
                        }
                        else {
                            throw sjtu::exception("", "Invalid query type");
                        }
                        break;
                    }
                }
            }
            ticketSystem.QueryTransfer(date, from, to, _type, time_stamp);
        }
        else if (cmd == "buy_ticket") {
            std::string tmp, username, trainID, date, from, to;
            int num; bool que = false;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'u': {
                        ss >> username;
                        break;
                    }
                    case 'i': {
                        ss >> trainID;
                        break;
                    }
                    case 'd': {
                        ss >> date;
                        break;
                    }
                    case 'n': {
                        ss >> num;
                        break;
                    }
                    case 'f': {
                        ss >> from;
                        break;
                    }
                    case 't': {
                        ss >> to;
                        break;
                    }
                    case 'q': {
                        ss >> tmp;
                        if (tmp == "true") {
                            que = true;
                        }
                        else if (tmp == "false") {
                            que = false;
                        }
                        else {
                            throw sjtu::exception("", "Invalid query type!");
                        }
                        break;
                    }
                }
            }
            ticketSystem.BuyTicket(username, trainID, date, from, to, num, que, time_stamp);
        }
        else if (cmd == "query_order") {
            std::string tmp, username;
            ss >> tmp >> username;
            ticketSystem.QueryOrder(username);
        }
        else if (cmd == "refund_ticket") {
            std::string tmp, username;
            int num;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'u': {
                        ss >> username;
                        break;
                    }
                    case 'n': {
                        ss >> num;
                        break;
                    }
                }
            }
            ticketSystem.RefundTicket(username, num, time_stamp);
        }
        else {
            throw sjtu::exception("", "Invalid command!");
        }
    }
    return 0;
}