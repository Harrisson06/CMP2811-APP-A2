#pragma once
#include "person.h"

class LibStaff :public Person {
public:
    LibStaff(int id, std::string name);
    std::string asString() override;
};