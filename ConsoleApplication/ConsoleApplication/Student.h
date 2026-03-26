#pragma once
#include "Person.h"

class Student : public Person {
public:
    Student(int id, string name);
    string asString() override;
};