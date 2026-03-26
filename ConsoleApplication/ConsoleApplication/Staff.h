#pragma once
#include "person.h"

class Staff : public Person {
public:
    Staff(int id, string name);
    string asString() override;
};