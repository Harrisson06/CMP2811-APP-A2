#include "Resource.h"

Resource::Resource(string id, string title, bool canLend) {
    _ID = id;
    _title = title;
    _canlend = canLend;
    _isBorrowed = false;
}