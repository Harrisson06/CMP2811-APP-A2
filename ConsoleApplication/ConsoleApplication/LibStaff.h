#pragma once
#include "person.h"

class LibStaff :public Person {
public:
    LibStaff(int id, string name);
    string asString() override;
};