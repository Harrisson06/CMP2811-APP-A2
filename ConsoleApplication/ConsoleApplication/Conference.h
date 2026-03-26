#pragma once
#include "Resource.h"

class Conference : public Resource {
private:
    string _acronym;

public:
    Conference(string id, string title, string acronym);
    string getAcronym() {return _acronym; }
    string asString() override;
};