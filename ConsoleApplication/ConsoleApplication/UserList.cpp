#include "UserList.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

UserList::UserList() {}

// Constructor loads users from file
UserList::UserList(string filename) {
    ifstream file(filename);
    string line;
    int userCount = 0;

    // Read file line by line
    while (getline(file, line)) {
        // IF: Line is empty | Skip
        if (line.empty())
            continue;

        // Increment user counter for unique ID assignment
        userCount++;

		// First character indicates user type (1=Student, 2=Staff, 3=LibStaff)
        char type = line[0];
        // Extract user name
        string name = line.substr(1);

        // IF: User type is "1", create new Student
        if (type == '1') {
            _u_list.push_back(new Student(userCount, name));
        }
        // ELSE IF: User type is "2", create new Staff
        else if (type == '2') {
            _u_list.push_back(new Staff(userCount, name));
        }
        // ELSE IF: User type is "3", create new LibStaff
        else if (type == '3') {
            _u_list.push_back(new LibStaff(userCount, name));
        }
    }
    file.close();
}

// Helper function to print all users
void UserList::printUserList() {
    for (auto u : _u_list) {
        cout << u->asString() << endl;
    }
}