#pragma once 
#include <string>

// Abstract base class representing a library resource
class Resource {
protected:
	std::string _ID;         // Unique identifier for the resource
	std::string _title;      // Title of the resource
	bool _canlend;      // Indicates if the resource can be borrowed
	bool _isBorrowed;   // Current borrow status

public:
    // Constructor to initialize resource with ID, title, leading perms
    Resource(std::string id, std::string title, bool canlend);

	// Virtual destructor | cleanup in derived classes if needed
	virtual ~Resource() {}
	// Pure virtual function | subclasses must implement this to return string representation
    virtual std::string asString() = 0;

    // Getters
	std::string getID() const { return _ID; }                   // Returns resource ID
	std::string getTitle() const { return _title; }			 // Returns resource title	
	bool getCanLend() const { return _canlend; }			 // Returns if resource can be borrowed
	bool getIsBorrowed() const { return _isBorrowed; }	 // Returns borrow status

    // Setters | Updates wether resource is currently borrowed
    void setIsBorrowed(bool b) { _isBorrowed =b; }
};