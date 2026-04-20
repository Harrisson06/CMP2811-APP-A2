#include "ResourceList.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

ResourceList::ResourceList() {}

// Constructor that loads resources from file
ResourceList::ResourceList(string filename) {
    ifstream file(filename);
    string line;

    // Counters for each resource type to generate unique IDs
    int bookCount = 0;
    int journalCount = 0;
    int confCount = 0;
    int section = 0; // tracks which section of the file is being read

    // Read file line by line
    while (getline(file, line)) {
        // IF: Line is a section separator (#####), increment section counter and continue
        if (line.substr(0, 5) == "#####") {
            section++;
            continue;
        }

        // IF: Line is a comment or empty, skip it
        if (line.empty() || line[0] == '#')
            continue;

        // Section 2 = Books
        if (section == 2) {
            // Books format: Author | Title | Year (3 consecutive lines)
            string author = line;
            string title, year;
            getline(file, title);
            getline(file, year);

            bookCount++;
            string id = "B00" + to_string(bookCount);
            _r_list.push_back(new Book(id, title, author));
        }

        // Section 3 = Journals
        else if (section == 3) {
            if (line.find(':') == string::npos) {
                // No colon = journal title
                journalCount++;
                string id = "J00" + to_string(journalCount);
                _r_list.push_back(new Journal(id, line));
            }
            else {
                // Colon = volume:issue line, attach to last journal
                if (!_r_list.empty()) {
                    Journal* j = dynamic_cast<Journal*>(_r_list.back());
                    if (j != nullptr) {
                        j->addVolumeIssue(line);
                    }
                }
            }
        }

        // Section 4 = Conferences
        else if (section == 4) {
            if (line.find(' ') != string::npos) {
                // Contains spaces = conference title
                confCount++;
                string id = "C00" + to_string(confCount);
                _r_list.push_back(new Conference(id, line));
            }
            else {
                // No spaces = acronym, attach to last conference
                if (!_r_list.empty()) {
                    Conference* c = dynamic_cast<Conference*>(_r_list.back());
                    if (c != nullptr) {
                        c->addAcronym(line);
                    }
                }
            }
        }
    } 

    file.close(); 
}

// Helper function to print all non-borrowed resources
void ResourceList::printResourceList() {
    for (auto r : _r_list) {
        // IF: Resource is not currently borrowed
        if (!r->getIsBorrowed()) {
            cout << r->asString() << endl;
        }
    }
}
