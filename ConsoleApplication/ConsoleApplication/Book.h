#pragma once
#include "Resource.h"

class Book : public Resource {
private:
    string _author;
public:
    Book(string id, string title, string author);
    string getAuthor() {return _author; }
    string asString() override;
};