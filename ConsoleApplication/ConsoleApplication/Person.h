#pragma once
#include <string>
using namespace std;

class Person {
protected:
    int _ID;
    string _name;
    int _borrow_limit;
public:
    Person(int id, string name, int borrow_limit);
    virtual string asString() = 0;

    // Getters
    int getID() {return _ID; }
    string getName() {return _name; }
    int getBorrowlimit() {return _borrow_limit; }
};