#include "UserList.h"
#include <fstream>
#include <iostream>
#include <string>

UserList::UserList() {}

// Constructor loads users from file
UserList::UserList(std::string filename) {
    std::ifstream file(filename);
    std::string line;
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
        std::string name = line.substr(1);

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
        std::cout << u->asString() << std::endl;
    }
}

Person* UserList::findUser(int userID) const {
	auto it = find_if(_u_list.begin(), _u_list.end(), [&](Person* p) {
		return p->getID() == userID;
		});
	return it != _u_list.end() ? *it : nullptr;
}