#include "Journal.h"

Journal::Journal(std::string id, std::string title) 
    : Resource(id, title, true) {
}

std::string Journal::asString() {
    std::string viStr = "";
    for (int i = 0; i < _volumeIssues.size(); i++) {
        if (i > 0) viStr += ", ";
        viStr += _volumeIssues[i];
    }
    std::string result = "ID: " + _ID + " | Title: " + _title;
    if (!viStr.empty()) {
        result += " | Volume/Issue: " + viStr;
    }
    return result;
}