#pragma once
#include "person.h"
#include "Resource.h"

// Represents a loan transaction linking a user to a borrowed resource
class Loan {
private:
	Person* _per;       // Pointer to user who borrowed the resource
	Resource* _res;	    // Pointer to the resource that was borrowed

public:
    // Constructor to create a new loan record
    Loan(Person* per, Resource* res);

    // Getters
    Person* getPerson() {return _per; }         // Returns pointer to user who made loan
	Resource* getResource() { return _res; } 	// Returns pointer to resource that was loaned

    // Returns string representation of the loan transaction
    string asString();
};