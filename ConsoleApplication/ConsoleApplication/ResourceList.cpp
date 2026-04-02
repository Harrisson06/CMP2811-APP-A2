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
            // Journals: Title 
            if (line.find(':') == string::npos) {
                string title = line;
                string temp;

                journalCount++;
                string id = "J00" + to_string(journalCount);
                _r_list.push_back(new Journal(id, title));
            }
        }

        else if (section == 4) {
            // conference: title | acronyms
            if (line.find(' ') != string::npos) {

                string title = line;
                confCount++;
                string id = "C00" + to_string(confCount);
                _r_list.push_back(new Conference(id, title, ""));
            } else {
                if (_r_list.empty()) {
                    Conference* last = dynamic_cast<Conference*>(_r_list.back());
                    cout << "trying to set acro: " << line << endl;
                    if (last != nullptr && last->getAcronym() == "") {
                        cout << "Cast success" << endl;
                        last->setAcronym(line);
                    }
                    else {
                        cout << "Cast fail" << endl;
                    }
                }
            }
        }
    }
    file.close();
}

void ResourceList::printResourceList() {
    for (auto r : _r_list) {
        cout << r->asString() << endl;
    }
}