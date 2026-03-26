#pragma once
#include "Resource.h"

class Journal : public Resource {
public:
    Journal(string id, string title);
    string asString() override;
};