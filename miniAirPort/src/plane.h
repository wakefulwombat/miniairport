#pragma once
#include "common.h"
#include "base_objectBase.h"

enum class PlaneCode {
	BLU,
	GRN,
	GRY,
	ORG,
	PNK,
	PUR,
	RED,
	WBL,
	WHT,
	YLW
};

enum class PlaneStatus {
	Preparating,
	Transporting,
	Waiting,
	Takeoffing,
	Landing
};

class Plane : public ObjectBase, public ImageProperty, public MoveProperty{
private:
	unsigned int id;
	PlaneCode code;
	PlaneStatus status;

public:
	Plane(Vec2D world_pos) : ObjectBase(world_pos, 1000, std::make_shared<InternalObjectController_None>()), ImageProperty(std::make_shared<ImagePropertyData>(Size(64,64))), MoveProperty(std::make_shared<MovePropertyData>()) { this->initialize(); }
	void initialize() override;
	void update() override;
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override;
};