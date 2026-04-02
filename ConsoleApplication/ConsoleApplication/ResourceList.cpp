#include "ResourceList.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

ResourceList::ResourceList() {}

ResourceList::ResourceList(string filename) {
    ifstream file(filename);
    string line;
    int bookCount = 0;
    int journalCount = 0;
    int confCount = 0;
    int section = 0;

    while (getline(file, line)) {
        if (line.substr(0, 5) == "#####") {
            section++;
            continue;
        }
        if (line[0] == '#' || line.empty()) continue;

        if (section == 2) {
            // Books: Author | Title | Year 
            string author = line;
            string title, year;
            getline(file, title);
            getline(file, year);
            bookCount++;
            string id = "B00" + to_string(bookCount);
            _r_list.push_back(new Book(id, title, author));
        }
        else if (section == 3) {
            //Conference: title | Volume: issue
            string title = line;
            string issues;
            getline(file, issues);
            journalCount++;
            string id ="J00" + to_string(journalCount);
            _r_list.push_back(new Journal(id, title));
        }
        else if (section == 4) {
            // conference: title | acronyms
            string title = line;
            string acronym;
            getline(file, acronym);
            confCount++;
            string id = "C00" + to_string(confCount);
            _r_list.push_back(new Conference(id, title, acronym));
        }
    }
    file.close();
}

void ResourceList::printResourceList() {
    for (auto r : _r_list) {
        cout << r->asString() << endl;
    }
}