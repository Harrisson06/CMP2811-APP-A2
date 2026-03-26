#include "Staff.h"

Staff::Staff(int id, string name) 
    : Person(id, name, 2) {
}

string Staff::asString() {
    return "ID: " + to_string(_ID) + " | Name: " + _name + " | Type: Staff | Borrow Limit: 2";
}