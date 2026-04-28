#pragma once
#include "Person.h"

class Student : public Person {
public:
    Student(int id, std::string name);
    std::string asString() override;
};