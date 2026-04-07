#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "ResourceList.h"
#include "UserList.h"
#include "Loan.h"
#include "Person.h"

using namespace std;

int main() {
    // Loads data from files
    ResourceList resourceList("A2resourceList.txt");
    UserList userList("A2userlist.txt");

    cout << "<|| Welcome to UniLib: The University Library Management System ||>" << endl;
    cout << "Borrow [userID] [resourceID]" << endl;
    cout << "Return [userID] [resourceID]" << endl;
    cout << "list : All resources available for endling" << endl;
    cout << "report [1/2] : (1) All resources currently loaned out, (2) All users which have borrowed a resource" << endl;
    cout << "Search [keyword]: Search for resources containing the keyword in the author/title/acronym" << endl;
    cout << "History: ordered list of borrowing/returning" << endl;

    vector<Loan*> loans;
    string input;

    while (true) {
        cout << "\n>>> ";
        getline(cin, input);

        istringstream iss(input);
        string command;
        iss >> command;

		// Exit the program
        if (command == "exit") {
            break;
        }

		// Lets user list all available resources;
        // Ascending | Descending
        else if (command == "list") {
            string sortOption;
            iss >> sortOption;

            vector<Resource*> available;
            for (auto r : resourceList.getList()) {
                if (!r->getIsBorrowed()) available.push_back(r);
            }

            if (sortOption == "ascending") {
                sort(available.begin(), available.end(), [](Resource* a, Resource* b) {
                    return a->getTitle() < b->getTitle();
                });
            }
            else if (sortOption == "descending") {
				sort(available.begin(), available.end(), [](Resource* a, Resource* b) {
					return a->getTitle() > b->getTitle();
					});
            }

            for (auto r : available) {
				cout << r->asString() << endl;
            }
        }

		// Prints a list of resources;
		// Currently loaned out | Users who have borrowed resources
        else if (command == "report") {
            int type;
            iss >> type;
            if (type == 1) {
                string sortOption;
                iss >> sortOption;

				vector<Resource*> loaned;
                for (auto loan : loans) {
                    loaned.push_back(loan->getResource());
                }

                if (sortOption == "ascending") {
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
            }
            else if (type == 2) {
                cout << "Users who have borrowed resources:" << endl;
                for (auto loan : loans) {
                    cout << loan->getPerson()->asString() << endl;
                }

                // Save file?
				cout << "Do you want to save this report to a file? (y/n) ";
                string save;
                getline(cin, save);
                if (save == "y" || save == "Y") {
                    ofstream report("Report.txt");
					report << "Users who have borrowed resources:" << endl;
                    for (auto loan : loans) {
						report << loan->getPerson()->asString() << endl;
                    }
                    report.close();
					cout << "Report saved to Report.txt" << endl;
                }
            }
        }

        else if (command == "borrow") {
            int userID;
            string resourceID;
            iss >> userID >> resourceID;

            // Finding user
            Person* user = nullptr;
            for (auto u : userList.getList()) {
                if (u->getID() == userID) {
                    user = u;
                    break;
                }
            }

            // Finding resource
            Resource* resource = nullptr;
            for (auto r : resourceList.getList()) {
                if (r->getID() == resourceID) {
                    resource = r;
                    break;
                }
            }
            int userLoanCount = 0;
            for (auto& loan : loans) {
                if (loan->getPerson()->getID() == user->getID()) userLoanCount++;
            }

            if (user == nullptr) {
                cout << "User not found" << endl;
            }

            else if (resource == nullptr) {
                cout << "Resource not found" << endl;
            }

			else if (user->getBorrowlimit() == 0) {
				cout << "User " << userID << " is not allowed to borrow resource" << endl;
			}

            else if (!resource->getCanLend() || resource->getIsBorrowed()) {
                cout << "Resource " << resourceID << " is currently unable to be borrowed" << endl;
            }
        
            else if (userLoanCount >= user->getBorrowlimit()) {
                cout << "User " << userID << " has reached their borrowing limit" << endl;
            }


            else {
                resource->setIsBorrowed(true);
                loans.push_back(new Loan(user, resource));
                cout << "Resource has been borrowed" << endl;
                // Saving borrow history
				ofstream history("History.txt", ios::app);
				history << "Borrowed: User " << userID << " borrowed Resource " << resourceID << endl;
                history.close();
            }
        }

        else if (command == "return") {
            int userID;
            string resourceID;
            iss >> userID >> resourceID;

            for (auto& loan : loans) {
                if (loan->getPerson()->getID() == userID &&
                    loan->getResource()->getID() == resourceID) {
                    loan->getResource()->setIsBorrowed(false);
                    loans.erase(remove(loans.begin(), loans.end(), loan), loans.end());
                    cout << "Resource has been returned" << endl;
                    // Saving return history
					ofstream history("history.txt", ios::app);
					history << "Returned: User " << userID << " returned Resource " << resourceID << endl;
					history.close();
                    break;
                }
            }
        }
        else if (command == "history") {
			ifstream history("history.txt");
			if (history.is_open()) {
				string line;
                cout << "Borrowing and Return history: " << endl;
				while (getline(history, line)) {
					cout << line << endl;
				}
				history.close();
			}
			else {
				cout << "No history available." << endl;
			}
        }
        else if (command == "search") {
            string keyword;
            iss >> keyword;

            transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);

            vector<Resource*> results;

            for (auto r : resourceList.getList()) {
				string str = r->asString();
				transform(str.begin(), str.end(), str.begin(), ::tolower);
                if (str.find(keyword) != string::npos) {
                    results.push_back(r);
                }
            }

			// Sort alphabetically by Title
            sort(results.begin(), results.end(), [](Resource* a, Resource* b) {
                return a->getTitle() < b->getTitle();
                });

            cout << "Search results for \"" << keyword << "\":" << endl;
            if (results.empty()) {
                cout << "No results found" << endl;
            }
            else {
				for (auto r : results) {
					cout << r->asString() << endl;
				}
            }
        }

		// Error handle for nothing matching the command
        else {
            cout << "unknown command." << endl;
        }
    }
    return 0;
}