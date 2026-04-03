#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "ResourceList.h"
#include "UserList.h"
#include "Loan.h"
#include "Person.h"
#include <algorithm>

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

        if (command == "exit") {
            break;
        }

        else if (command == "list") {
            resourceList.printResourceList();
        }

        else if (command == "report") {
            int type;
            iss >> type;
            if (type == 1) {
                cout << "Resources currently loaned out:" << endl;
                for (auto loan : loans) {
                    cout << loan->getResource()->asString() << endl;
                }
            }
            else if (type == 2) {
                cout << "Users who have borrowed resources:" << endl;
                for (auto loan : loans) {
                    cout << loan->getPerson()->asString() << endl;
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
                    break;
                }
            }
        }

        else {
            cout << "unknown command." << endl;
        }
    }
    return 0;
}