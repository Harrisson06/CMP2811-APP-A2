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

int main() {
    // Loads data from files
    ResourceList resourceList("A2ResourceList.txt");
    UserList userList("A2UserList.txt");

    // Display welcome message and instructions for commands
    cout << "<|| Welcome to UniLib: The University Library Management System ||>" << endl;
    cout << "Borrow [userID] [resourceID]" << endl;
    cout << "Return [userID] [resourceID]" << endl;
    cout << "list : All resources available for endling" << endl;
    cout << "report [1/2] : (1) All resources currently loaned out, (2) All users which have borrowed a resource" << endl;
    cout << "Search [keyword]: Search for resources containing the keyword in the author/title/acronym" << endl;
    cout << "History: ordered list of borrowing/returning" << endl;

    // Smart pointer Vector to store all active loans
    vector<unique_ptr<Loan>> loans;
    string input;

    // Main loop | Runs until exit command is given
    while (true) {
        cout << "\n>>> ";
        getline(cin, input);

        // Parse the input command
        istringstream iss(input);
        string command;
        iss >> command;

		// IF: user enters "exit"
        if (command == "exit") {
            // Exit program
            break;
        }

		// ELSE IF: Lets user list all available resources;
        // Allows sorting in Ascending and Descending
        else if (command == "list") {
            string sortOption;
            iss >> sortOption;

            // Create vector of available resources 
            vector<Resource*> available;
            for (auto r : resourceList.getList()) {
				// IF: Resource is not currently loaned out, add to available list
                if (!r->getIsBorrowed()) available.push_back(r);
            }

            // IF: User selected ascending sort order (A-Z)
            if (sortOption == "ascending") {
                sort(available.begin(), available.end(), [](Resource* a, Resource* b) {
                    return a->getTitle() < b->getTitle();
                });
            }

			// ELSE IF: User selected descending sort order (Z-A)
            else if (sortOption == "descending") {
				sort(available.begin(), available.end(), [](Resource* a, Resource* b) {
					return a->getTitle() > b->getTitle();
					});
            }

            // Display all available resources
            for (auto r : available) {
				cout << r->asString() << endl;
            }
        }

		// ELSE IF: Uer wants a report of resources currently loaned out, or users who have borrowed resources
        else if (command == "report") {
            int type;
            iss >> type;
            if (type == 1) {
                string sortOption;
                iss >> sortOption;

                // Collects all resources that are loaned out
				vector<Resource*> loaned;
                for (auto& loan : loans) {
                    loaned.push_back(loan->getResource());
                }

				// IF: User selected ascending sort order (A-Z)
                if (sortOption == "ascending") {
					sort(loaned.begin(), loaned.end(), [](Resource* a, Resource* b) {
						return a->getTitle() < b->getTitle();
						});
				}
                
				// ELSE IF: User selected descending sort order (Z-A)
				else if (sortOption == "descending") {
					sort(loaned.begin(), loaned.end(), [](Resource* a, Resource* b) {
						return a->getTitle() > b->getTitle();
						});
				}

                cout << "Resources currently loaned out:" << endl;
                for (auto r : loaned) {
                    cout << r->asString() << endl;
                }

				// Prompt user to save the report to a .txt file
				cout << "Do you want to save this report to a file? (y/n) ";
				string save;    
				getline(cin, save);

                if (save == "y" || save == "Y") {
                    // Create/overwrite LoaneReport.txt file and write loaned resources list
                    ofstream report("Report.txt");
                    report << "Resources currently loaned out:" << endl;

                    for (auto r : loaned) {
                        report << r->asString() << endl;
                    }
                    report.close();
                    cout << "Report saved to Report.txt" << endl;
                }
            }

            // ELSE IF: Report type 2 | Shows all users who have borrowed resources
            else if (type == 2) {
                cout << "Users who have borrowed resources:" << endl;
                for (auto& loan : loans) {
                    cout << loan->getPerson()->asString() << endl;
                }

                // Prompt user to save the report to a .txt file
				cout << "Do you want to save this report to a file? (y/n) ";
                string save;
                getline(cin, save);

				// IF: User enteres "y" or "Y", save the report to "Report.txt"
                if (save == "y" || save == "Y") {
					// Create/overwrite Report.txt file and writes user list
                    ofstream report("Report.txt");
					report << "Users who have borrowed resources:" << endl;

                    for (auto& loan : loans) {
						report << loan->getPerson()->asString() << endl;
                    }
                    report.close();
					cout << "Report saved to Report.txt" << endl;
                }
            }
        }

		// ELSE IF: User wants to borrow a resource
        else if (command == "borrow") {
            int userID;
            string resourceID;
            iss >> userID >> resourceID;

            // Finding user by ID
            Person* user = nullptr;
            for (auto u : userList.getList()) {
                if (u->getID() == userID) {
                    user = u;
                    break;
                }
            }

            // Finding resource by ID
            Resource* resource = nullptr;
            for (auto r : resourceList.getList()) {
                if (r->getID() == resourceID) {
                    resource = r;
                    break;
                }
            }

            // VALIDATION: Check if user exits in system
            if (user == nullptr) {
                cout << "User not found" << endl;
            }

            // VALIDATION: Check if resource exists in system
            else if (resource == nullptr) {
                cout << "Resource not found" << endl;
            }
            else {
				// Counts how many resources the user currently has borrowed
				int userLoanCount = 0;
				for (auto& loan : loans) {
					if (loan->getPerson()->getID() == user->getID()) userLoanCount++;
				}
            

			    // VALIDATION: Check if user has borrow persission (borrow limit > 0)
			    if (user->getBorrowlimit() == 0) {
				    cout << "User " << userID << " is not allowed to borrow resource" << endl;
			    }

                // VALIDATION: Check if resource can be borrowed
                else if (!resource->getCanLend() || resource->getIsBorrowed()) {
                    cout << "Resource " << resourceID << " is currently unable to be borrowed" << endl;
                }
        
			    // VALIDATION: Check if user has reached their borrow limit
                else if (userLoanCount >= user->getBorrowlimit()) {
                    cout << "User " << userID << " has reached their borrowing limit" << endl;
                }

			    // ALL VALIDATIONS PASSED: Proceed with borrowing the resource
                else {
                    // Marks resource as borrowed
                    resource->setIsBorrowed(true);
                    // Creates new loan record
                    loans.push_back(make_unique<Loan>(user, resource));
                    cout << "Resource has been borrowed" << endl;

                    // Appends borrow transaction to history.txt
                    ofstream history("History.txt", ios::app);
                    history << "Borrowed: User " << userID << " borrowed Resource " << resourceID << endl;
                    history.close();
                }
            }
        }

        // ELSE IF: User wants to return a borrowed resource
        else if (command == "return") {
            int userID;
            string resourceID;
            iss >> userID >> resourceID;

            // Search through active loans to find matching user | resource
            for (auto& loan : loans) {
                // IF: Found Matching user | resource
                if (loan->getPerson()->getID() == userID &&
                    loan->getResource()->getID() == resourceID) {

					// Marks resource as returned
                    loan->getResource()->setIsBorrowed(false);
					// Remove the loan from active loans list
                    loans.erase(remove(loans.begin(), loans.end(), loan), loans.end());
                    cout << "Resource has been returned" << endl;


                    // Append return transaction to history.txt
					ofstream history("History.txt", ios::app);
					history << "Returned: User " << userID << " returned Resource " << resourceID << endl;
					history.close();
                    break;
                }
            }
        }

        // ELSE IF: User wants to view transaction history
        else if (command == "history") {
			// Opens history.txt
			ifstream history("History.txt");

            // IF: history.txt opened successfully
			if (history.is_open()) {
				string line;
                cout << "Borrowing and Return history: " << endl;
				// Reads each line from history.txt and displays it
				while (getline(history, line)) {
					cout << line << endl;
				}
				history.close();
			}
			// ELSE: History file doesnt exist or cannot be opened
			else {
				cout << "No history available." << endl;
			}
        }

        // ELSE IF: User wants to search for a resource by keyword
        else if (command == "search") {
            string keyword;
            iss >> keyword;

			// Convert search keyword to lowercase for case-insensitive search
            transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);

            // Vector stores search results
            vector<Resource*> results;

            // Search through all resources fro keyword match
            for (auto r : resourceList.getList()) {
				// Convers resource string representation to lowercase
				string str = r->asString();
				transform(str.begin(), str.end(), str.begin(), ::tolower);

				// IF: Keyword found in resource string, add to search results
                if (str.find(keyword) != string::npos) {
                    results.push_back(r);
                }
            }

			// Sort alphabetically by Title
            sort(results.begin(), results.end(), [](Resource* a, Resource* b) {
                return a->getTitle() < b->getTitle();
                });

            cout << "Search results for \"" << keyword << "\":" << endl;

            // IF: No search results found
            if (results.empty()) {
                cout << "No results found" << endl;
            }
			// ELSE: Display search results
            else {
				for (auto r : results) {
					cout << r->asString() << endl;
				}
            }
        }

		// ELSE: no command is recognized
        else {
            cout << "unknown command." << endl;
        }
    }
    return 0;
}