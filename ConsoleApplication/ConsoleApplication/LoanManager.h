#pragma once

#include "UserList.h"
#include "Loan.h"
#include "ResourceList.h"
#include <vector>
#include <string>
#include <memory>

// Handles all loan-related operations
// Encapsulates loan creation, return, counting and overdue tracking
class LoanManager {
private:
	std::vector<std::unique_ptr<Loan>> _loans; // Stores all active loans
public:
	LoanManager();

	// Borrow a resource for a user
	std::string borrowResource(Person* user, Resource* resource);

	// Return a resource for a user
	std::string returnResource(int userID, std::string resourceID);

	// Get all loans (for report)
	const std::vector<std::unique_ptr<Loan>>& getLoans() const { return _loans; }

	// Count loans for a specific user
	int countUserLoans(int userID) const;

	// Get all overdue loans
	std::vector<Loan*> getOverdueLoans() const;
};