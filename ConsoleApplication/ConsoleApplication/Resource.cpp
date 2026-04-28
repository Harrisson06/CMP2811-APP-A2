#include "Resource.h"

Resource::Resource(std::string id, std::string title, bool canLend) {
    _ID = id;
    _title = title;
    _canlend = canLend;
    _isBorrowed = false;
}