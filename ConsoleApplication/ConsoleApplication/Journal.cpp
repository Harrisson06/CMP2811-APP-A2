#include "Journal.h"

Journal::Journal(string id, string title) 
    : Resource(id, title, true) {
}

string Journal::asString() {
    string viStr = "";
    for (int i = 0; i < _volumeIssues.size(); i++) {
        if (i > 0) viStr += ", ";
        viStr += _volumeIssues[i];
    }
    string result = "ID: " + _ID + " | Title: " + _title;
    if (!viStr.empty()) {
        result += " | Volume/Issue: " + viStr;
    }
    return result;
}