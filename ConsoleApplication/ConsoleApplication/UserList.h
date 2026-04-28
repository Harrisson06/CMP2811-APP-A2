#pragma once
#include "Person.h"
#include "Student.h"
#include "Staff.h"
#include "LibStaff.h"
#include <vector>
#include <string>

class UserList {
private:
    std::vector<Person*> _u_list;
public:
    UserList();
    UserList(std::string filename);
    void printUserList();
    std::vector<Person*> getList() {return _u_list; }
};