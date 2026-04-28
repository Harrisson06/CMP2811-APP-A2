#pragma once
#include "Resource.h"
#include <string>

class Book : public Resource {
private:
    std::string _author;
public:
    Book(std::string id, std::string title, std::string author);
    std::string getAuthor() const {return _author; }
    std::string asString() override;
};