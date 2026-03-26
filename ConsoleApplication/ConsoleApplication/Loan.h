#pragma once
#include "person.h"
#include "Resource.h"

class Loan {
private:
    Person* _per;
    Resource* _res;
public:
    Loan(Person* per, Resource* res);

    // Getters
    Person* getPerson() {return _per; }
    Resource* getResource() {return _res; }

    string asString();
};