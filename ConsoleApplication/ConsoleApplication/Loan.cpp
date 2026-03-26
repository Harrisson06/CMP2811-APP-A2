#include "Loan.h"

Loan::Loan(Person* per, Resource* res) {
    _per = per;
    _res = res;
}

string Loan::asString() {
    return "user: " + _per->getName() + " | Resource: " + _res->getTitle();
}