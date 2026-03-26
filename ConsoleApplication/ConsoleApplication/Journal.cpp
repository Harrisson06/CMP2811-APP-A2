#include "Journal.h"

Journal::Journal(string id, string title) 
    : Resource(id, title, true) {
}

string Journal::asString() {
    return "ID: " + _ID + " | Title: " + _title;
}