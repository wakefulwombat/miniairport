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
	const unsigned int _id;
	const TargetMarkerKind kind;

public:
	TargetMarker(Vec2D center_world_pos, unsigned int id, TargetMarkerKind kind) : ObjectBase(center_world_pos, 11), ImageProperty(std::make_shared<ImagePropertyData>(Size(256, 256))), kind(kind), _id(id) {}
	void update() override {}
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override { if (!this->validation) return; camera->drawImageInWorld(this->world_pos, Vec2D(127, 210), Resources::getImage()->getTargetMarker(), this->img_prop); }
	int getID() { return _id; }

	void setControlRights(ControlStatus status) override { this->control_status = status; }
};