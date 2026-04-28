#include "Student.h"

Student::Student(int id, std::string name)
    : Person(id, name, 1) {
}

std::string Student::asString() {
    return "ID: " + std::to_string(_ID) + " | Name: " + _name + " | Type: Student | Borrow Limit: 1";
}