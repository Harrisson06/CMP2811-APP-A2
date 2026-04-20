#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <memory>

#include "ResourceList.h"
#include "UserList.h"
#include "Loan.h"
#include "Person.h"

using namespace std;

// Helper function to find a user by ID
Person* findUser(vector<Person*> users, int userID) {
    auto it = find_if(users.begin(), users.end(), [&](Person* p) {
        return p->getID() == userID;
        });
    return it != users.end() ? *it : nullptr;
}

// Helper function to find a resource by ID
Resource* findResource(vector<Resource*>& resources, string resourceID) {
    auto it = find_if(resources.begin(), resources.end(), [&](Resource* r) {
        return r->getID() == resourceID;
        });
    return it != resources.end() ? *it : nullptr;
}

// Helper function to save a report to a file
void saveReport(vector<string> lines, string filename) {
    ofstream report(filename);
    for (auto& line : lines) {
        report << line << endl;
    }
    report.close();
    cout << "Report saved to " << filename << endl;
}

// Helper: prompts user for ascending/descending and returns true if ascending
bool askSortOrder() {
    cout << "ascending or descending? ";
    string order;
    getline(cin, order);
    // Convert to lowercase for case-insensitive comparison
    transform(order.begin(), order.end(), order.begin(), ::tolower);
    return order == "ascending";
}

// Helper: sorts a resource vector by author (Books) or title (others)
void sortByAuthor(vector<Resource*>& list, bool ascending) {
    sort(list.begin(), list.end(), [&](Resource* a, Resource* b) {
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

int main() {
    // Load data from files
    ResourceList resourceList("A2ResourceList.txt");
    UserList userList("A2UserList.txt");

    // Display welcome message and command instructions
    cout << "<|| Welcome to UniLib: The University Library Management System ||>" << endl;
    cout << "Commands:" << endl;
    cout << "  borrow [userID] [resourceID]" << endl;
    cout << "  return [userID] [resourceID]" << endl;
    cout << "  list [ascending|descending|author]  : All resources available for lending" << endl;
    cout << "  report 1 [ascending|descending|author] : Resources currently loaned out" << endl;
    cout << "  report 2                               : All users who have borrowed a resource" << endl;
    cout << "  search [keyword]                       : Search by title / author / acronym" << endl;
    cout << "  history                                : Ordered list of borrowing/returning activity" << endl;
    cout << "  exit" << endl;

    // Smart pointer vector to store all active loans
    vector<unique_ptr<Loan>> loans;
    string input;

    // Main loop — runs until exit command
    while (true) {
        cout << "\n>>> ";
        getline(cin, input);

        istringstream iss(input);
        string command;
        iss >> command;

        // Convert command to lowercase for case-insensitive matching
        transform(command.begin(), command.end(), command.begin(), ::tolower);

        // EXIT
        if (command == "exit") {
            break;
        }

        // LIST: all available (non-borrowed) resources, with optional sort
        else if (command == "list") {
            string sortOption;
            iss >> sortOption;
            transform(sortOption.begin(), sortOption.end(), sortOption.begin(), ::tolower);

            vector<Resource*> available;
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

            cout << "\nAvailable Resources:" << endl;
            for (auto r : available) {
                cout << r->asString() << endl;
            }
        }

        // REPORT: loaned resources (1) or users who borrowed (2)
        else if (command == "report") {
            int type;
            iss >> type;

            if (type == 1) {
                string sortOption;
                iss >> sortOption;
                transform(sortOption.begin(), sortOption.end(), sortOption.begin(), ::tolower);

                vector<Resource*> loaned;
                for (auto& loan : loans) {
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

                cout << "Resources currently loaned out:" << endl;
                for (auto r : loaned) {
                    cout << r->asString() << endl;
                }

                cout << "Save this report to a file? (y/n) ";
                string save;
                getline(cin, save);
                if (save == "y" || save == "Y") {
                    vector<string> lines;
                    lines.push_back("Resources currently loaned out:");
                    for (auto r : loaned) lines.push_back(r->asString());
                    saveReport(lines, "LoanedReport.txt"); 
                }
            }

            else if (type == 2) {
                cout << "Users who have borrowed resources:" << endl;
                for (auto& loan : loans) {
                    cout << loan->getPerson()->asString() << endl;
                }

                cout << "Save this report to a file? (y/n) ";
                string save;
                getline(cin, save);
                if (save == "y" || save == "Y") {
                    vector<string> lines;
                    lines.push_back("Users who have borrowed resources:");
                    for (auto& loan : loans) lines.push_back(loan->getPerson()->asString());
                    saveReport(lines, "UsersReport.txt");
                }
            }
        }

        // BORROW: lend a resource to a user
        else if (command == "borrow") {
            int userID;
            string resourceID;

            // VALIDATION: check numeric userID was provided
            if (!(iss >> userID)) {
                cout << "Invalid user ID. Usage: borrow [userID] [resourceID]" << endl;
                continue;
            }
            if (!(iss >> resourceID)) {
                cout << "Invalid resource ID. Usage: borrow [userID] [resourceID]" << endl;
                continue;
            }

            auto uList = userList.getList();
            auto rList = resourceList.getList();

            Resource* resource = findResource(rList, resourceID);
            Person* user = findUser(uList, userID);

            if (user == nullptr) {
                cout << "User not found." << endl;
            }
            else if (resource == nullptr) {
                cout << "Resource not found." << endl;
            }
            else {
                // Count how many resources this user currently has borrowed
                int userLoanCount = 0;
                for (auto& loan : loans) {
                    if (loan->getPerson()->getID() == user->getID()) userLoanCount++;
                }

                if (user->getBorrowlimit() == 0) {
                    cout << "User " << userID << " is not permitted to borrow resources." << endl;
                }
                else if (!resource->getCanLend() || resource->getIsBorrowed()) {
                    cout << "Resource " << resourceID << " is not available to borrow." << endl;
                }
                else if (userLoanCount >= user->getBorrowlimit()) {
                    cout << "User " << userID << " has reached their borrowing limit." << endl;
                }
                else {
                    resource->setIsBorrowed(true);
                    loans.push_back(make_unique<Loan>(user, resource));
                    cout << "Resource " << resourceID << " successfully borrowed by user " << userID << "." << endl;

                    // Append to history file
                    ofstream history("History.txt", ios::app);
                    history << "Borrowed: User " << userID << " borrowed Resource " << resourceID << endl;
                    history.close();
                }
            }
        }

        // RETURN: return a borrowed resource
        else if (command == "return") {
            int userID;
            string resourceID;

            // VALIDATION: check numeric userID was provided
            if (!(iss >> userID)) {
                cout << "Invalid user ID. Usage: return [userID] [resourceID]" << endl;
                continue;
            }
            if (!(iss >> resourceID)) {
                cout << "Invalid resource ID. Usage: return [userID] [resourceID]" << endl;
                continue;
            }

            auto it = remove_if(loans.begin(), loans.end(), [&](const unique_ptr<Loan>& l) {
                return l->getPerson()->getID() == userID && l->getResource()->getID() == resourceID;
                });

            if (it != loans.end()) {
                // Mark resource as available before erasing the loan record
                for (auto i = it; i != loans.end(); ++i) {
                    (*i)->getResource()->setIsBorrowed(false);
                }
                loans.erase(it, loans.end());
                cout << "Resource " << resourceID << " successfully returned by user " << userID << "." << endl;

                // Append to history file
                ofstream history("History.txt", ios::app);
                history << "Returned: User " << userID << " returned Resource " << resourceID << endl;
                history.close();
            }
            else {
                cout << "No matching loan found for user " << userID << " and resource " << resourceID << "." << endl;
            }
        }

        // SEARCH: find resources by keyword in title, author, or acronym
        else if (command == "search") {
            string keyword;
            iss >> keyword;

            if (keyword.empty()) {
                cout << "Usage: search [keyword]" << endl;
                continue;
            }

            // Lowercase keyword for case-insensitive matching
            transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);

            vector<Resource*> results;
            for (auto r : resourceList.getList()) {
                string str = r->asString();
                transform(str.begin(), str.end(), str.begin(), ::tolower);
                if (str.find(keyword) != string::npos) {
                    results.push_back(r);
                }
            }

            // Sort results alphabetically by title
            sort(results.begin(), results.end(), [](Resource* a, Resource* b) {
                return a->getTitle() < b->getTitle();
                });

            cout << "Search results for \"" << keyword << "\":" << endl;
            if (results.empty()) {
                cout << "No results found." << endl;
            }
            else {
                for (auto r : results) {
                    cout << r->asString() << endl;
                }
            }
        }

        // HISTORY: display the borrowing/returning activity log
        else if (command == "history") {
            ifstream historyFile("History.txt");
            if (historyFile.is_open()) {
                cout << "Borrowing/Returning History:" << endl;
                string line;
                while (getline(historyFile, line)) {
                    cout << line << endl;
                }
                historyFile.close();
            }
            else {
                cout << "No history recorded yet." << endl;
            }
        }

        // UNKNOWN COMMAND
        else {
            cout << "Unknown command. Type 'exit' to quit." << endl;
        }
    }

    return 0;
}