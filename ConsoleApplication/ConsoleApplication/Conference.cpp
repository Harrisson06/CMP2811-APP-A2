#include "Conference.h"

Conference::Conference(string id, string title, string acronym) 
    : Resource(id, title, false) {
    _acronym = acronym;
}

string Conference::asString() {
    return "ID: " + _ID + " | Title: " + _title + " | Acronym: " + _acronym;
}