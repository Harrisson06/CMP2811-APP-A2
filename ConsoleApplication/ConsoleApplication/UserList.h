#pragma once
#include "Person.h"
#include "Student.h"
#include "Staff.h"
#include "LibStaff.h"
#include <vector>
#include <string>
using namespace std;

class UserList {
private:
    vector<Person*> _u_list;
public:
    UserList();
    UserList(string filename);
    void printUserList();
    vector<Person*> getList() {return _u_list; }
};