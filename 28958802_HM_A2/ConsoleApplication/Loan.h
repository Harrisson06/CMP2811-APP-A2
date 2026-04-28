#pragma once
#include "person.h"
#include "Resource.h"
#include <string>
#include <ctime>

// Represents a loan transaction linking a user to a borrowed resource
class Loan {
private:
	Person* _per;       // Pointer to user who borrowed the resource
	Resource* _res;	    // Pointer to the resource that was borrowed
	time_t _dueDate;    // Due date for returning the resource

public:
    // Constructor to create a new loan record
    Loan(Person* per, Resource* res);

    // Getters
    Person* getPerson() const {return _per; }         // Returns pointer to user who made loan
	Resource* getResource() const { return _res; } 	  // Returns pointer to resource that was loaned
	time_t getDueDate() const { return _dueDate; }    // Returns due date for returning the resource
    std::string getDueDateString() const;             // Returns due date as a human-readable string
	bool isOverdue() const;                           // Checks if the loan is overdue based on current date

    // Returns string representation of the loan transaction
    std::string asString() const;
};