#pragma once 
#include <string>
using namespace std;

// Abstract base class representing a library resource
class Resource {
protected:
	string _ID;         // Unique identifier for the resource
	string _title;      // Title of the resource
	bool _canlend;      // Indicates if the resource can be borrowed
	bool _isBorrowed;   // Current borrow status

public:
    // Constructor to initialize resource with ID, title, leading perms
    Resource(string id, string title, bool canlend);

	// Virtual destructor | cleanup in derived classes if needed
	virtual ~Resource() {}
	// Pure virtual function | subclasses must implement this to return string representation
    virtual string asString() = 0;

    // Getters
	string getID() { return _ID; }                   // Returns resource ID
	string getTitle() { return _title; }			 // Returns resource title	
	bool getCanLend() { return _canlend; }			 // Returns if resource can be borrowed
	bool getIsBorrowed() { return _isBorrowed; }	 // Returns borrow status

    // Setters | Updates wether resource is currently borrowed
    void setIsBorrowed(bool b) { _isBorrowed =b; }
};