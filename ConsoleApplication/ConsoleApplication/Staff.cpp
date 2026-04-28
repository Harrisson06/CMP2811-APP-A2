#include "Staff.h"

Staff::Staff(int id, std::string name) 
    : Person(id, name, 2) {
}

std::string Staff::asString() {
    return "ID: " + std::to_string(_ID) + " | Name: " + _name + " | Type: Staff | Borrow Limit: 2";
}