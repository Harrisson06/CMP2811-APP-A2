#pragma once
#include "Resource.h"
#include <vector>
#include <string>

class Conference : public Resource {
private:
    std::vector<std::string> _acronyms; // Stores multiple acronyms for this conference

public:
    Conference(std::string id, std::string title);
    void addAcronym(std::string a) { _acronyms.push_back(a); }

    // Adds an acronym to the list
    std::vector<std::string> getAcronyms() const { return _acronyms; }

    // Returns all acronyms
    std::string getAcronym() { return _acronyms.empty() ? "" : _acronyms[0]; }

    // Returns first acronym (for search compat)
    std::string asString() override;
};