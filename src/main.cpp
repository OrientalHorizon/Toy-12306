//
// Created by iakno on 2023/5/23.
//
#include <utils.h>
#include "UserSystem.h"
#include "TokenScanner.h"
#include "BPT.h"
using std::cin;
using std::cout;
using std::endl;

UserSystem userSystem;

signed main() {
    std::string line;
    std::string cmd;
    bool is_first = true;
    while (getline(std::cin, line)) {
        std::stringstream ss;
        ss.clear();
        ss << line;
        ss >> cmd;
        // Time stamp
        cout << cmd << endl;
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
                for (int unused = 0; unused < 4; ++unused) {
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
                    }
                    privilege = 10;
                    userSystem.AddUser("", username, password, name, mailAddr, privilege);
                }
                is_first = false;
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
            std::string tmp = "", cur_username = "", username = "", password = "", name = "", mailAddr = "";
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
            std::string tmp, trainID, stationNum, seatNum, stations, prices, startTime, travelTimes, stopoverTimes, saleDate, type;
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
                        ss >> type;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}