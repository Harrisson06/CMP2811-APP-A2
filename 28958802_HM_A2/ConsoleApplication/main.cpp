// Student Name: Harrison Macdonald 
// Student Number: 28958802

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <memory>
#include <ctime>

#include "ResourceList.h"
#include "UserList.h"
#include "Loan.h"
#include "Person.h"
#include "LoanManager.h"

// Helper function to save a report to a file
void saveReport(std::vector<std::string> lines, std::string filename) {
    std::ofstream report(filename);
    for (auto& line : lines) {
        report << line << std::endl;
    }
    report.close();
    std::cout << "Report saved to " << filename << std::endl;
}

// Helper: prompts user for ascending/descending and returns true if ascending
bool askSortOrder() {
    std::cout << "ascending or descending? ";
    std::string order;
    getline(std::cin, order);
    // Convert to lowercase for case-insensitive comparison
    transform(order.begin(), order.end(), order.begin(), ::tolower);
    return order == "ascending";
}

// Helper: sorts a resource vector by author (Books) or title (others)
void sortByAuthor(std::vector<Resource*>& list, bool ascending) {
    sort(list.begin(), list.end(), [&](Resource* a, Resource* b) {
        if (a == nullptr || b == nullptr) return false;
        Book* ba = dynamic_cast<Book*>(a);
        Book* bb = dynamic_cast<Book*>(b);
        if (ba && bb) {
            return ascending ? ba->getAuthor() < bb->getAuthor()
                : ba->getAuthor() > bb->getAuthor();
        }
        // Non-books fall back to title sort
        return ascending ? a->getTitle() < b->getTitle()
            : a->getTitle() > b->getTitle();
        });
}

// Helper: gets current timestamp as string
std::string getTimestamp() {
	time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
	std::string timestamp = std::to_string(1900 + ltm.tm_year) + "-" +
		               std::to_string(1 + ltm.tm_mon) + "-" +
		               std::to_string(ltm.tm_mday) + " " +
		               std::to_string(ltm.tm_hour) + ":" +
		               std::to_string(ltm.tm_min) + ":" +
		               std::to_string(ltm.tm_sec);
	return timestamp;
}

int main() {
    // Load data from files
    ResourceList resourceList("A2ResourceList.txt");
    UserList userList("A2UserList.txt");

    // Display welcome message and command instructions
    std::cout << "<|| Welcome to UniLib: The University Library Management System ||>" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  borrow [userID] [resourceID]" << std::endl;
    std::cout << "  return [userID] [resourceID]" << std::endl;
    std::cout << "  list [ascending|descending|author]  : All resources available for lending" << std::endl;
    std::cout << "  report 1 [ascending|descending|author] : Resources currently loaned out" << std::endl;
    std::cout << "  report 2                               : All users who have borrowed a resource" << std::endl;
    std::cout << "  search [keyword]                       : Search by title / author / acronym" << std::endl;
    std::cout << "  history                                : Ordered list of borrowing/returning activity" << std::endl;
    std::cout << "  overdue                                : List all overdue loans" << std::endl;
    std::cout << "  exit" << std::endl;

    // Smart pointer vector to store all active loans
    LoanManager loanManager;
    std::string input;

    // Main loop — runs until exit command
    while (true) {
        std::cout << "\n>>> ";
        getline(std::cin, input);

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        // Convert command to lowercase for case-insensitive matching
        transform(command.begin(), command.end(), command.begin(), ::tolower);

        // EXIT
        if (command == "exit") {
            break;
        }

        // LIST: all available (non-borrowed) resources, with optional sort
        else if (command == "list") {
            std::string sortOption;
            iss >> sortOption;
            std::transform(sortOption.begin(), sortOption.end(), sortOption.begin(), ::tolower);

            std::vector<Resource*> available;
            for (auto r : resourceList.getList()) {
                if (!r->getIsBorrowed()) available.push_back(r);
            }

            if (sortOption == "author") {
                bool asc = askSortOrder(); // BUG FIX: was comparing "Ascending" vs lowercase input
                sortByAuthor(available, asc);
            }
            else if (sortOption == "ascending") {
                sort(available.begin(), available.end(), [](Resource* a, Resource* b) {
                    return a->getTitle() < b->getTitle();
                    });
            }
            else if (sortOption == "descending") {
                sort(available.begin(), available.end(), [](Resource* a, Resource* b) {
                    return a->getTitle() > b->getTitle();
                    });
            }

            std::cout << "\nAvailable Resources:" << std::endl;
            for (auto r : available) {
                std::cout << r->asString() << std::endl;
            }
        }

        // REPORT: loaned resources (1) or users who borrowed (2)
        else if (command == "report") {
            int type;
            iss >> type;

            if (type == 1) {
                std::string sortOption;
                iss >> sortOption;
                std::transform(sortOption.begin(), sortOption.end(), sortOption.begin(), ::tolower);

                std::vector<Resource*> loaned;
                for (auto& loan : loanManager.getLoans()) {
                    loaned.push_back(loan->getResource());
                }

                if (sortOption == "author") {
                    bool asc = askSortOrder();
                    sortByAuthor(loaned, asc);
                }
                else if (sortOption == "ascending") {
                    sort(loaned.begin(), loaned.end(), [](Resource* a, Resource* b) {
                        return a->getTitle() < b->getTitle();
                        });
                }
                else if (sortOption == "descending") {
                    sort(loaned.begin(), loaned.end(), [](Resource* a, Resource* b) {
                        return a->getTitle() > b->getTitle();
                        });
                }

                std::cout << "Resources currently loaned out:" << std::endl;
                for (const auto& loan : loanManager.getLoans()) {
                    std::cout << loan->asString() << std::endl;
                }

                std::cout << "Save this report to a file? (y/n) ";
                std::string save;
                getline(std::cin, save);
                if (save == "y" || save == "Y") {
                    std::vector<std::string> lines;
                    lines.push_back("Resources currently loaned out:");
                    for (auto r : loaned) lines.push_back(r->asString());
                    saveReport(lines, "LoanedReport.txt"); 
                }
            }

            else if (type == 2) {
                std::cout << "Users who have borrowed resources:" << std::endl;
                for (auto& loan : loanManager.getLoans()) {
                    std::cout << loan->getPerson()->asString() << std::endl;
                }

                std::cout << "Save this report to a file? (y/n) ";
                std::string save;
                getline(std::cin, save);
                if (save == "y" || save == "Y") {
                    std::vector<std::string> lines;
                    lines.push_back("Users who have borrowed resources:");
                    for (const auto& loan : loanManager.getLoans()) {
                        lines.push_back(loan->asString());
					}
                    saveReport(lines, "UsersReport.txt");
                }
            }
        }

        // BORROW: lend a resource to a user
        else if (command == "borrow") {
            int userID;
            std::string resourceID;

            // VALIDATION: check numeric userID was provided
            if (!(iss >> userID)) {
                std::cout << "Invalid user ID. Usage: borrow [userID] [resourceID]" << std::endl;
                continue;
            }
            if (!(iss >> resourceID)) {
                std::cout << "Invalid resource ID. Usage: borrow [userID] [resourceID]" << std::endl;
                continue;
            }

            Person* user = userList.findUser(userID);
			Resource* resource = resourceList.findResource(resourceID);

            if (user == nullptr) {
                std::cout << "User not found." << std::endl;
            }
            else if (resource == nullptr) {
                std::cout << "Resource not found." << std::endl;
            }
            else {
				std::string result = loanManager.borrowResource(user, resource);
                std::cout << result << std::endl;

                    // Log to history is success
					if (result.find("successfully borrowed") != std::string::npos) {
						std::ofstream history("History.txt", std::ios::app);
						history << "[" << getTimestamp() << "] Borrowed: User " << userID << " borrowed Resource " << resourceID << std::endl;
						history.close();
					}
                }
            }

        // RETURN: return a borrowed resource
        else if (command == "return") {
            int userID;
            std::string resourceID;

            // VALIDATION: check numeric userID was provided
            if (!(iss >> userID)) {
                std::cout << "Invalid user ID. Usage: return [userID] [resourceID]" << std::endl;
                continue;
            }
            if (!(iss >> resourceID)) {
                std::cout << "Invalid resource ID. Usage: return [userID] [resourceID]" << std::endl;
                continue;
            }

			std::string result = loanManager.returnResource(userID, resourceID);
            std::cout << result << std::endl;

            if (result.find("successfully") != std::string::npos) {
                std::ofstream history("History.txt", std::ios::app);
                history << "[" << getTimestamp() << "] Returned: User " << userID << " returned Resource " << resourceID << std::endl;
				history.close();
            }
        }

        // SEARCH: find resources by keyword in title, author, or acronym
        else if (command == "search") {
            std::string keyword;
            iss >> keyword;

            if (keyword.empty()) {
                std::cout << "Usage: search [keyword]" << std::endl;
                continue;
            }

            // Lowercase keyword for case-insensitive matching
            transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);

            std::vector<Resource*> results;
            for (auto r : resourceList.getList()) {
                std::string str = r->asString();
                transform(str.begin(), str.end(), str.begin(), ::tolower);
                if (str.find(keyword) != std::string::npos) {
                    results.push_back(r);
                }
            }

            // Sort results alphabetically by title
            sort(results.begin(), results.end(), [](Resource* a, Resource* b) {
                return a->getTitle() < b->getTitle();
                });

            std::cout << "Search results for \"" << keyword << "\":" << std::endl;
            if (results.empty()) {
                std::cout << "No results found." << std::endl;
            }
            else {
                for (auto r : results) {
                    std::cout << r->asString() << std::endl;
                }
            }
        }

        // HISTORY: display the borrowing/returning activity log
        else if (command == "history") {
            std::ifstream historyFile("History.txt");
            if (historyFile.is_open()) {
                std::cout << "Borrowing/Returning History:" << std::endl;
                std::string line;
                while (getline(historyFile, line)) {
                    std::cout << line << std::endl;
                }
                historyFile.close();
            }
            else {
                std::cout << "No history recorded yet." << std::endl;
            }
        }

        // ELSE IF: Display all overdue loans
        else if (command == "overdue") {
			auto overdueLoans = loanManager.getOverdueLoans();
            std::cout << "Overdue Loans:" << std::endl;
			if (overdueLoans.empty()) {
				std::cout << "No overdue loans at this time." << std::endl;
				continue;
			}
        }

        // UNKNOWN COMMAND
        else {
            std::cout << "Unknown command. Type 'exit' to quit." << std::endl;
        }
    }

    return 0;
}