#pragma once

#include "base_objectBase.h"
#include "common.h"
#include <memory>

class MousePointer : public ImageProperty, public ObjectBase {
private:
	bool move_rocked;

	Vec2D rock_moveStart, rock_moveEnd;
	double rock_movePixel, rock_moveAngle;

	Vec2D remember_pos;

	void setControlRights(ControlStatus status) override final { this->control_status = status; }

public:
	MousePointer();
	void initialize() override;
	void update() override;
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override;

	void rememberHide();
	void rememberShow();
};