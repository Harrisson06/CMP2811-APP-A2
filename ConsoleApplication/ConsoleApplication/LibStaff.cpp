#include "LibStaff.h"

LibStaff::LibStaff(int id, std::string name) 
    : Person(id, name, 0) {
}

std::string LibStaff::asString() {
    return "ID: " + std::to_string(_ID) + " | Name: " + _name + " | Type: Library Staff | Borrow Limit: 0";
}