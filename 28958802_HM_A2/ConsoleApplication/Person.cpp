#include "Person.h"

Person::Person(int id, std::string name, int borrow_limit) {
    _ID = id;
    _name = name;
    _borrow_limit = borrow_limit;
}