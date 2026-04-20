#pragma once
#include "Resource.h"
#include <vector>

class Conference : public Resource {
private:
    vector<string> _acronyms; // Stores multiple acronyms for this conference

public:
    Conference(string id, string title);
    void addAcronym(string a) { _acronyms.push_back(a); }

    // Adds an acronym to the list
    vector<string> getAcronyms() { return _acronyms; }

    // Returns all acronyms
    string getAcronym() { return _acronyms.empty() ? "" : _acronyms[0]; }

    // Returns first acronym (for search compat)
    string asString() override;
};