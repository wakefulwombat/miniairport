#include "timeTable.h"
#include <fstream>
#include <sstream>
#include <string>

TimeTable::TimeTable(std::function<void(std::shared_ptr<ObjectBase>)> addObject, std::function<Size(void)> getWorldSize, std::function<bool(void)> isHighSpeedNow) : addObject(addObject), getWorldSize(getWorldSize), isHighSpeedNow(isHighSpeedNow) {
	this->timer = std::make_shared<TimerBox>(Vec2D(1220, 30));
	this->arrivals = std::make_shared<Table>(addObject, 50, 30);
	this->departures = std::make_shared<Table>(addObject, 50, 30);
	this->initialize();
}

void TimeTable::update() { 
	for (auto it = this->planes.begin(); it != this->planes.end();) { 
		if (!(*it)->getValidation()) {
			it = this->planes.erase(it);
			continue; 
		}
		++it;
	} 
	for (auto it = this->target_markers.begin(); it != this->target_markers.end();) {
		if (!(*it)->getValidation()) {
			it = this->target_markers.erase(it); 
			continue;
		}
		++it;
	}

	for (auto it = this->infos_timeTable.begin(); it != this->infos_timeTable.end(); ++it) {
		if ((*it).is_departure) continue;
		if ((!this->timer->getPreTime().isSameHourAndMinute((*it).estimated_time - Time24(0, 20, 0))) && (this->timer->getNowTime().isSameHourAndMinute((*it).estimated_time - Time24(0, 20, 0)))) {
			Vec2D start = Vec2D::getRandomOnCircle(5.0 * 60 * 50);
			std::shared_ptr<Plane> p = std::make_shared<Plane>(start + (this->getWorldSize() / 2).toVec(), this->id_counter_plane, (*it).plane_code, PlaneStatus::Flying, this->isHighSpeedNow, 2.0, M_PI + start.toAngle());
			this->planes.push_back(p);
			this->addObject(p);
			this->id_counter_plane++;
		}
	}
	
}