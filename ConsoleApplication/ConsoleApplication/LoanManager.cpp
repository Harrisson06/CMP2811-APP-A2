#include "LoanManager.h"
#include <fstream>
#include <ctime>
#include <algorithm>

// Default constructor | initializes empty loan list
LoanManager::LoanManager() {}

// Resource counter for a specific user
// Iterates through all loans and counts how many belong to the given userID
int LoanManager::countUserLoans(int userID) const {
	int count = 0;
	for (const auto& loan : _loans) {
		if (loan->getPerson()->getID() == userID) count++;
	}
	return count;
}

// Validates a borrow request
// Checks permission | availability | user loan count against borrow limit
std::string LoanManager::borrowResource(Person* user, Resource* resource) {
	// Checks if user is allowed to borrow (libstaff have a limit of 0)
	if (user->getBorrowlimit() == 0) {
		return "User " + std::to_string(user->getID()) + " is not permitted to borrow resources.";
	}
	// Check if resource can be lent and isnt currently borrowed
	if (!resource->getCanLend() || resource->getIsBorrowed()) {
		return "Resource " + resource->getID() + " is not available to borrow>";
	}
	// Checks if user hits borrow limit
	if (countUserLoans(user->getID()) >= user->getBorrowlimit()) {
		return "User " + std::to_string(user->getID()) + " has reached their borrowing limit.";
	}
	
	// All validations passed | create loan
	resource->setIsBorrowed(true);
	_loans.push_back(std::make_unique<Loan>(user, resource));
	return "Resource " + resource->getID() + " successfully borrowed by user " + std::to_string(user->getID()) + ".";
}


// Validate return request 
// Uses STL remove_if to find matching loan
std::string LoanManager::returnResource(int userID, std::string resourceID) {
	auto it = std::remove_if(_loans.begin(), _loans.end(), [&](const std::unique_ptr<Loan>& l) {
		return l->getPerson()->getID() == userID && l->getResource()->getID() == resourceID;
		});

	// IF: Matching loan found | Mark resource as available and erase loan record 
	if (it != _loans.end()) {
		for (auto i = it; i != _loans.end(); ++i) {
			(*i)->getResource()->setIsBorrowed(false);
		}
		_loans.erase(it, _loans.end());
		return "Resource " + resourceID + " successfully returned by user " + std::to_string(userID) + ".";
	}
	return "Loan not found.";
}


// Returns a vector of pointers to all overdue loans 
// Iterates through loans and collects loans past due date
std::vector<Loan*> LoanManager::getOverdueLoans() const {
	std::vector<Loan*> overdue;
	for (const auto& loan : _loans) {
		if (loan->isOverdue()) {
			overdue.push_back(loan.get());
		}
	}
	return overdue;
}