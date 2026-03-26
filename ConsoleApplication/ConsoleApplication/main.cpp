#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "ResourceList.h"
#include "UserList.h"
#include "Loan.h"
using namespace std;

int main() {
    // Loads data from files
    ResourceList resourceList("A2resourceList.txt");
    UserList userList("A2userlist.txt");

    cout << "### wWelcome to UniLib: The University Library Management System ###" << endl;
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
                cout << "Resources currentyl loaned out:" << endl;
                for (auto loan : loans) {
                    cout << loan->getPerson()->asString() << endl;
                }
            }
        }
        else {
            cout << "unknown command." << endl;
        }
    }
    return 0;
}