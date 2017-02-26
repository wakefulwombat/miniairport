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
	const unsigned int id;
	const PlaneCode code;
	PlaneStatus status;

public:
	Plane(Vec2D world_pos, unsigned int id, PlaneCode code, PlaneStatus status) : ObjectBase(world_pos, 1000), ImageProperty(std::make_shared<ImagePropertyData>(Size(64, 64),0.3)), MoveProperty(std::make_shared<MovePropertyData>()), id(id), code(code) { this->initialize(); this->status = status; }
	void initialize() override;
	void update() override;
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override;
};



class PlaneFactory {
private:
	unsigned int id_counter;
	const std::function<void(std::shared_ptr<ObjectBase>)> addObject;

public:
	PlaneFactory(std::function<void(std::shared_ptr<ObjectBase>)> addObject) : addObject(addObject) { this->initialize(); }
	void initialize() { this->id_counter = 0; }

	void makeDeparturePlane(unsigned int marker_id, PlaneCode code);//初期準備段階でユーザーが作る出発機の生成
	void makeArrivalPlaneByTime(Time24 now_time);//タイムテーブルに沿って出現する到着機の生成
};