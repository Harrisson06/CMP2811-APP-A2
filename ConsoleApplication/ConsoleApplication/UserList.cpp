#include "UserList.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

UserList::UserList() {}

UserList::UserList(string filename) {
    ifstream file(filename);
    string line;
    int userCount = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;

        userCount++;
        char type = line[0];
        string name = line.substr(1);

        if (type == '1') {
            _u_list.push_back(new Student(userCount, name));
        }
        else if (type == '2') {
            _u_list.push_back(new Staff(userCount, name));
        }
        else if (type == '3') {
            _u_list.push_back(new LibStaff(userCount, name));
        }
    }
    file.close();
}

void UserList::printUserList() {
    for (auto u : _u_list) {
        cout << u->asString() << endl;
    }
}