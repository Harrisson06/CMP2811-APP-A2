#include "Book.h"

Book::Book(string id, string title, string author)
    : Resource(id, title, true) {
        _author = author;
    }

string Book::asString() {
    return "ID: " + _ID + " | Title: " + _title + " | Author: " + _author;
}