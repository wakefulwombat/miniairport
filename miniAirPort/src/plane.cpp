#include "plane.h"
#include "resource.h"

Plane::Plane(Vec2D world_pos, unsigned int id, PlaneCode code, PlaneStatus status, std::function<bool(void)> isHighSpeedNow, double vel, double rad)
		: ObjectBase(world_pos, 1000), 
		  ImageProperty(std::make_shared<ImagePropertyData>(Size(256, 256), 0.8, rad)),
		  MoveProperty(std::make_shared<MovePropertyData>(vel, rad)), _id(id), code(code), isHighSpeedNow(isHighSpeedNow) {
	this->initialize();
	this->status = status;
	this->target_marker_id = -1;
	this->time_table_id = -1;
}


void Plane::initialize() {

}

void Plane::update() {
	if (this->status == PlaneStatus::FreeFlightComing) {
		if(this->isHighSpeedNow()) this->world_pos += Vec2D::fromPowAng(this->mv_prop->trans_vel*5.0, this->mv_prop->trans_angle);
		else this->world_pos += Vec2D::fromPowAng(this->mv_prop->trans_vel, this->mv_prop->trans_angle);
	}

	if (this->status == PlaneStatus::FreeFlightAway) {
		if (this->isHighSpeedNow()) this->world_pos += Vec2D::fromPowAng(this->mv_prop->trans_vel*5.0, this->mv_prop->trans_angle);
		else this->world_pos += Vec2D::fromPowAng(this->mv_prop->trans_vel, this->mv_prop->trans_angle);
	}
}

void Plane::draw(const std::shared_ptr<CameraDrawInterface> &camera) const {
	camera->drawImageInWorld(this->world_pos, Resources::getImage()->getPlane(this->code), this->img_prop);
}