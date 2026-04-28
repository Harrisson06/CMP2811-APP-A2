#pragma once
#include "person.h"

class Staff : public Person {
public:
    Staff(int id, std::string name);
    std::string asString() override;
};