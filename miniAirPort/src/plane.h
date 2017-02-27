#pragma once
#include "common.h"
#include "base_objectBase.h"
#include <vector>
#include <functional>

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
	const unsigned int _id;
	const PlaneCode code;
	PlaneStatus status;
	unsigned int target_marker_id, time_table_id;

public:
	Plane(Vec2D world_pos, unsigned int id, PlaneCode code, PlaneStatus status) : ObjectBase(world_pos, 1000), ImageProperty(std::make_shared<ImagePropertyData>(Size(64, 64), 0.3)), MoveProperty(std::make_shared<MovePropertyData>()), _id(id), code(code) { this->initialize(); this->status = status; this->target_marker_id = -1; this->time_table_id = -1; }
	void initialize() override;
	void update() override;
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override;

	void associateWithTargetMarker(unsigned int target_marker_id) { this->target_marker_id = target_marker_id; }
	void associateWithTimeTable(unsigned int time_table_id) { this->time_table_id = time_table_id; }
	unsigned int getAssociatedTargetMarkerID() { return this->target_marker_id; }
	unsigned int getAssociatedTimeTableID() { return this->time_table_id; }
	void resetRelationWithTargetMarker() { this->target_marker_id = -1; }
	void resetRelationWithTimeTable() { this->time_table_id = -1; }

	unsigned int getID() { return this->_id; }
};