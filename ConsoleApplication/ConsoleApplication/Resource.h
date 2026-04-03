#pragma once 
#include <string>
using namespace std;

class Resource {
protected:
    string _ID;
    string _title;
    bool _canlend;
    bool _isBorrowed;

public:
    Resource(string id, string title, bool canlend);
	virtual ~Resource() {}
    virtual string asString() = 0;

    // Getters
    string getID() {return _ID; }
    string getTitle() { return _title; }
    bool getCanLend() {return _canlend; }
    bool getIsBorrowed() {return _isBorrowed; }

    // Setters
    void setIsBorrowed(bool b) { _isBorrowed =b; }
};