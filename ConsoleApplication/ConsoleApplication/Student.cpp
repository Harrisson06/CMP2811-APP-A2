#include "Student.h"

Student::Student(int id, string name) 
    : Person(id, name, 1) {
}

string Student::asString() {
    return "ID: " + to_string(_ID) + " | Name: " + _name + " | Type: Student | Borrow Limit: 1";
}