#pragma once
#include "plane.h"
#include "targetMarker.h"
#include "common.h"
#include <memory>
#include <vector>
#include <functional>

struct TimeTableInfo {
public:
	unsigned int _id;
	Time24 estimated_time;
	PlaneCode plane_code;
	bool is_departure;
	bool is_cleared;
	unsigned int plane_id;
};

struct TargetMarkerInfo {
public:
	unsigned int _id;
	Vec2D pos;
	TargetMarkerKind kind;
};

class TimerBox : public ObjectBase {
private:
	Time24 now_time;
	int increment_milli_sec;

public:
	TimerBox(Vec2D pos) :ObjectBase(pos, 900000) {}
	void initialize() override {}
	void update() override { this->now_time += this->increment_milli_sec; }
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override { camera->drawStringOnWindowFixed(this->world_pos, this->now_time.toString(), Color_RGB(255, 255, 255)); }

	void setNowTime(Time24 now) { this->now_time = now; }
	void changeIncrementMilliSecond(int milli_sec) { this->increment_milli_sec = milli_sec; }
};

class TimeTable {
private:
	const std::function<void(std::shared_ptr<ObjectBase>)> addObject;

	std::shared_ptr<TimerBox> timer;

	std::vector<std::shared_ptr<Plane>> planes;
	std::vector<std::shared_ptr<TargetMarker>> target_markers;

	unsigned int id_counter_timeTable, id_counter_targetMarker, id_counter_plane;
	std::vector<TargetMarkerInfo> infos_targetMarker;
	std::vector<TimeTableInfo> infos_timeTable;

public:
	TimeTable(std::function<void(std::shared_ptr<ObjectBase>)> addObject) : addObject(addObject) { this->timer = std::make_shared<TimerBox>(Vec2D(1220, 30)); this->initialize(); }
	void initialize() { this->id_counter_targetMarker = 0; this->id_counter_timeTable; this->id_counter_plane = 0; this->infos_timeTable.clear(); this->infos_targetMarker.clear(); this->planes.clear(); this->target_markers.clear(); }
	void update();

	void addTimerBoxToObject() { this->addObject(this->timer); }
	void setNowTime(Time24 now) { this->timer->setNowTime(now); }
	void changeIncrementMilliSecond(int milli_sec) { this->timer->changeIncrementMilliSecond(milli_sec); }

	void addTargetMarkerInfo(Vec2D pos, TargetMarkerKind kind) { TargetMarkerInfo info; info._id = this->id_counter_targetMarker; info.pos = pos; info.kind = kind; this->infos_targetMarker.push_back(info); this->id_counter_targetMarker++; }
	void addTimeTableInfo(Time24 estimated_time, PlaneCode code, bool is_departure) { TimeTableInfo info; info._id = this->id_counter_timeTable; info.estimated_time = estimated_time; info.plane_code = code; info.is_departure = is_departure; info.is_cleared = false; info.plane_id = -1; this->infos_timeTable.push_back(info); this->id_counter_timeTable++; }

	void removePlaneByID(unsigned int plane_id) { for (auto it = this->planes.begin(); it != this->planes.end(); ++it) { if ((*it)->getID() == plane_id) { (*it)->setInvalid(); this->planes.erase(it); return; } } }
	void removeTargetMarkerAll() { for (auto it = this->target_markers.begin(); it != this->target_markers.end(); ++it) { (*it)->setInvalid(); this->target_markers.clear(); } }

	void associateWithPlane(unsigned int time_table_id, unsigned int plane_id) { for (auto it = this->infos_timeTable.begin(); it != this->infos_timeTable.end(); ++it) { if ((*it)._id == time_table_id) { (*it).plane_id = plane_id; return; } } }
	void resetRelationWithPlane(unsigned int time_table_id) { for (auto it = this->infos_timeTable.begin(); it != this->infos_timeTable.end(); ++it) { if ((*it)._id == time_table_id) { (*it).plane_id = -1; return; } } }
	
};