#include "Loan.h"

Loan::Loan(Person* per, Resource* res) {
    _per = per;
    _res = res;
	// Set due date to 14 days from now
	_dueDate = time(0) + (14 * 24 * 60 * 60); // 14 days in seconds
}

std::string Loan::getDueDateString() const {
	tm ltm;
	localtime_s(&ltm, &_dueDate);
	return std::to_string(ltm.tm_mday) + "/"
		 + std::to_string(1 + ltm.tm_mon) + "/" 
		 + std::to_string(1900 + ltm.tm_year);
}

bool Loan::isOverdue() const {
	return time(0) > _dueDate;
}

std::string Loan::asString() const {
	return _per->getName() + " | " + _res->getTitle() + " | Due: " + getDueDateString();
}