#pragma once
#include "base_objectBase.h"
#include "resource.h"
#include <functional>
#include <vector>

enum class TargetMarkerKind {
	Boarding,
	WaitingForTakeoff,
	LandingStart,
	LandingEnd
};

class TargetMarker : public ObjectBase, public ImageProperty{
private:
	const unsigned int id;
	const TargetMarkerKind kind;

public:
	TargetMarker(Vec2D center_world_pos, unsigned int id, TargetMarkerKind kind) : ObjectBase(center_world_pos, 11), ImageProperty(std::make_shared<ImagePropertyData>(Size(256, 256))), kind(kind), id(id) {}
	void update() override {}
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override { if (!this->validation) return; camera->drawImageInWorld(this->world_pos, Vec2D(127, 210), Resources::getImage()->getTargetMarker(), this->img_prop); }
	int getID() { return id; }

	void setControlRights(ControlStatus status) override { this->control_status = status; }
};


struct TargetMarkerInfo {
public:
	unsigned int id;
	Vec2D pos;
	TargetMarkerKind kind;
};

class TargetMarkerFactory {
private:
	unsigned int id_counter;
	const std::function<void(std::shared_ptr<ObjectBase>)> addObject;
	std::vector<TargetMarkerInfo> infos;
	std::vector<std::shared_ptr<ExternalMinimalController>> marker_remove;

public:
	TargetMarkerFactory(std::function<void(std::shared_ptr<ObjectBase>)> addObject) : addObject(addObject) { this->initialize(); }
	void initialize() { this->id_counter = 0; this->removeAllTargetMarker(); }
	void addTargetMarkerInfo(Vec2D pos, TargetMarkerKind kind) { TargetMarkerInfo info; info.id = this->id_counter; info.pos = pos; info.kind = kind; this->infos.push_back(info); this->id_counter++; }

	void makeTargetByKind(TargetMarkerKind kind) {
		for (auto it = this->infos.begin(); it != this->infos.end(); ++it) { 
			if ((*it).kind == kind) { 
				std::shared_ptr<TargetMarker> marker = std::make_shared<TargetMarker>((*it).pos, (*it).id, (*it).kind);
				this->marker_remove.push_back(marker);
				this->addObject(marker);
			}
		}
	}
	
	void makeTargetByID(int id) { for (auto it = this->infos.begin(); it != this->infos.end(); ++it) { if ((*it).id == id) { std::shared_ptr<TargetMarker> marker = std::make_shared<TargetMarker>((*it).pos, (*it).id, (*it).kind); this->marker_remove.push_back(marker); this->addObject(marker); return; } } }
	void removeAllTargetMarker() { for (auto it = this->marker_remove.begin(); it != this->marker_remove.end(); ++it) (*it)->setInvalid(); this->marker_remove.clear(); }

	std::vector<unsigned int> getIDList(TargetMarkerKind kind) { std::vector<unsigned int> v; for (auto it = this->infos.begin(); it != this->infos.end(); ++it) { if ((*it).kind == kind) v.push_back((*it).id); } return v; }
};