#include "Conference.h"

Conference::Conference(string id, string title) 
    : Resource(id, title, false) {
    // No acronyms added ny addAcronym function
}

string Conference::asString() {
    // Build acronym list as comma-seperated string
    string acronymStr = "";
	for (int i = 0; i < _acronyms.size(); i++) {
        if (i > 0) acronymStr += ", ";
        acronymStr += _acronyms[i];
		}
    return "ID: " + _ID + " | Title: " + _title + " | Acronyms: " + acronymStr;
	}