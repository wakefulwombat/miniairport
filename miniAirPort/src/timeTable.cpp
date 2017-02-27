#include "timeTable.h"
#include <fstream>
#include <sstream>
#include <string>

void TimeTable::update() { 
	for (auto it = this->planes.begin(); it != this->planes.end();) { 
		if (!(*it)->getValidation()) {
			it = this->planes.erase(it);
			continue; } ++it;
	} 
	for (auto it = this->target_markers.begin(); it != this->target_markers.end();) {
		if (!(*it)->getValidation()) {
			it = this->target_markers.erase(it); 
			continue;
		}
		++it;
	}


}