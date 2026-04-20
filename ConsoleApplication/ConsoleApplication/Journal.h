#pragma once
#include "Resource.h"
#include <vector>

class Journal : public Resource {
private:
    vector<string> _volumeIssues; 

public:
    Journal(string id, string title);
	void addVolumeIssue(string vi) { _volumeIssues.push_back(vi); }
	vector<string> getVolumeIssues() { return _volumeIssues; }
    string asString() override;
};