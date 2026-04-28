#include "Book.h"

Book::Book(std::string id, std::string title, std::string author)
    : Resource(id, title, true) {
        _author = author;
    }

std::string Book::asString() {
    return "ID: " + _ID + " | Title: " + _title + " | Author: " + _author;
}