#include "mousePointer.h"
#include "input.h"
#include "resource.h"
#include <math.h>

MousePointer::MousePointer() : ObjectBase(Input::getNowMousePoint(), 100000, std::make_shared<InternalObjectController_None>()), ImageProperty(std::make_shared<ImagePropertyData>(Size(32, 32))) {
	this->initialize();
}

void MousePointer::initialize() {
	this->move_rocked = false;
}

void MousePointer::update() {
	if (this->validation == false) return;

	if (this->move_rocked) {
		this->world_pos.x = this->rock_moveStart.x + this->rock_movePixel*cos(this->rock_moveAngle);
		this->world_pos.y = this->rock_moveStart.x + this->rock_movePixel*cos(this->rock_moveAngle);
		if (this->world_pos.getDistance(this->rock_moveEnd) < this->rock_movePixel) {
			this->move_rocked = false;
			this->world_pos = this->rock_moveEnd;
		}
		Input::setNowMousePoint(this->world_pos);
	}
	else {
		this->world_pos = Input::getNowMousePoint();
	}
}

void MousePointer::draw(const std::shared_ptr<CameraDrawInterface> &camera) const {
	if (this->validation == false) return;

	camera->drawImageOnWindow(this->world_pos, Vec2D(0.0, 0.0), Resources::getImage()->getMousePointer(), this->img_prop);
}

void MousePointer::rememberHide() {
	this->remember_pos = Input::getNowMousePoint();
	this->validation = false;
}

void MousePointer::rememberShow() {
	Input::setNowMousePoint(this->remember_pos);
	this->validation = true;
}