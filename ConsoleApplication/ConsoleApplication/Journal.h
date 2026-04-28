#pragma once
#include "Resource.h"
#include <vector>
#include <string>

class Journal : public Resource {
private:
    std::vector<std::string> _volumeIssues; 

public:
    Journal(std::string id, std::string title);
	void addVolumeIssue(std::string vi) { _volumeIssues.push_back(vi); }
    std::vector<std::string> getVolumeIssues() { return _volumeIssues; }
    std::string asString() override;
};