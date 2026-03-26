#include "LibStaff.h"

LibStaff::LibStaff(int id, string name) 
    : Person(id, name, 0) {
}

string LibStaff::asString() {
    return "ID: " + to_string(_ID) + " | Name: " + _name + " | Type: Library Staff | Borrow Limit: 0";
}