#pragma once
#include <string>
using namespace std;

// Abstract base class for all library users
class Person {
protected:
	int _ID;            // Unique identifier for user
	string _name;       // User's name
	int _borrow_limit;  // Max Number of resources user casn borrow simultaneously

public:
	// Constructor to initialize user attributes
    Person(int id, string name, int borrow_limit);

    // Pure virtual function | subclasses must implement this to return string represnetation
    virtual string asString() = 0;

    // Getters
	int getID() { return _ID; }                      // Returns user ID
    string getName() {return _name; }                // Returns user name
    int getBorrowlimit() {return _borrow_limit; }    // Returns user borrow limit
};