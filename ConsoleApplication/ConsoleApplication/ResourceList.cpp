#include "ResourceList.h"
#include <fstream>
#include <iostream>
#include <string>
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
        //IF: Line is a section separator (#####), increment section counter and continue to next line
        if (line.substr(0, 5) == "#####") {
            section++;
            continue;
        }

        // IF: Line is a comment or empty, skip it
        if (line[0] == '#' || line.empty())
            continue;

        // IF: Section 2 | Books
        if (section == 2) {
            // Books format: Author | Title | Year 
            string author = line;
            string title, year;

			// Read next two lines for title and year
            getline(file, title);
            getline(file, year);

            // Create unique Book ID
            bookCount++;
            string id = "B00" + to_string(bookCount);
            // Adds new book to resource list
            _r_list.push_back(new Book(id, title, author));
        }

		// ELSE IF: Section 3 | Journals
        else if (section == 3) {
            // Journals format: Title (colon: indicates details)

			// IF: Line doesnt contain a colon, its a journal title 
            if (line.find(':') == string::npos) {
                string title = line;
                string temp;

				// Create unique Journal ID
                journalCount++;
                string id = "J00" + to_string(journalCount);
				// Adds new journal to resource list
                _r_list.push_back(new Journal(id, title));
            }
        }

		// ELSE IF: Section 4 | Conferences
        else if (section == 4) {
            // conference format: title | acronyms

            // IF: Line contains spaces (conference title)
            if (line.find(' ') != string::npos) {
                string title = line;

                // Create unique Conference ID
                confCount++;
                string id = "C00" + to_string(confCount);
                // Adds new conference to resource list with empty acronym 
                _r_list.push_back(new Conference(id, title, ""));
            }
            
            // ELSE: Line has no spaces | acronym
            else {
                // IF: List is not empty
				if (!_r_list.empty()) {
                    // Get last added resource and cast it to conference type
					Conference* c = dynamic_cast<Conference*>(_r_list.back());

					// IF: Successfully cast to conference && Acronym is empty
					if (c != nullptr && c->getAcronym() == "") {
                        // Set acronym to this conference
						c->setAcronym(line);
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