#pragma once
#include "Resource.h"
#include "Book.h"
#include "Journal.h"
#include "Conference.h"
#include <vector>
#include <string>
using namespace std;

class ResourceList {
private:
    vector<Resource*> _r_list;
public:
    ResourceList();
    ResourceList(string filename);
    void printResourceList();
    vector<Resource*> getList() {return _r_list; }
};