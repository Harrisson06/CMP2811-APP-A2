#pragma once
#include "Resource.h"
#include "Book.h"
#include "Journal.h"
#include "Conference.h"
#include <vector>
#include <string>

class ResourceList {
private:
    std::vector<Resource*> _r_list;
public:
    ResourceList();
    ResourceList(std::string filename);
    void printResourceList();
    std::vector<Resource*> getList() {return _r_list; }
};