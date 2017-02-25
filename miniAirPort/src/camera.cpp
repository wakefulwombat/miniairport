#include "Camera.h"
#include "resource.h"
#include "DxLib.h"
#include <math.h>
#include <algorithm>

void Camera::init(Size window_size) {
	this->windowPixelSize = window_size;
	this->worldSize = this->windowPixelSize;
	this->pos_anchor_world_now = (this->windowPixelSize / 2).toVec();
	this->pos_anchor_window = (this->windowPixelSize / 2).toVec();
	this->shield_ratio = 1.0;

	this->anchor_target_world_pos_goal = this->pos_anchor_world_now;
	this->anchor_target_world_pos_start = this->pos_anchor_world_now;
	this->move_count = 0;
	this->move_count_max = 0;

	this->zoom_magnification_now = 1.0;
	this->zoom_count = 0;
	this->zoom_count_max = 0;
	this->zoom_magnification_end = 1.0;
	this->zoom_magnification_start = 1.0;

	this->rotate_angle_now = 0.0;
	this->rotate_count = 0;
	this->rotate_count_max = 0;
	this->rotate_angle_start = 0.0;
	this->rotate_angle_end = 0.0;

	this->sway_now = Vec2D(0.0, 0.0);
	this->sway_count = 0;
	this->sway_count_max = 0;
}

void Camera::setAnchorWorldPosition(Vec2D world_pos) {
	this->move_count = 0;
	this->move_count_max = 0;

	this->pos_anchor_world_now = world_pos;
	this->anchor_target_world_pos_goal = world_pos;
	this->anchor_target_world_pos_start = world_pos;
}

void Camera::setAnchorWorldPosition(Vec2D world_pos, int count) {
	this->move_count = 0;
	this->move_count_max = count;

	this->anchor_target_world_pos_goal = world_pos;
	this->anchor_target_world_pos_start = this->pos_anchor_world_now;
}

void Camera::setAnchorWindowPosition(Vec2D window_pos) {
	this->move_count = 0;
	this->move_count_max = 0;

	this->anchor_target_world_pos_goal = this->pos_anchor_world_now;
	this->anchor_target_world_pos_start = this->pos_anchor_world_now;
	this->pos_anchor_window = window_pos;
}

void Camera::setAnchorWindowPosition(Vec2D window_pos, int count) {
	this->move_count = 0;
	this->move_count_max = count;

	this->anchor_target_world_pos_goal = this->pos_anchor_world_now;
	this->anchor_target_world_pos_start = this->toWorldPosFromWindowPosPx(window_pos);
	this->pos_anchor_world_now = this->toWorldPosFromWindowPosPx(window_pos);
	this->pos_anchor_window = window_pos;
}

void Camera::setZoom(double zoom) {
	this->zoom_count = 0;
	this->zoom_count_max = 0;

	this->zoom_magnification_now = zoom;
	this->zoom_magnification_start = zoom;
	this->zoom_magnification_end = zoom;
}

void Camera::setZoom(double zoom, int count) {
	this->zoom_count = 0;
	this->zoom_count_max = count;

	this->zoom_magnification_start = this->zoom_magnification_now;
	this->zoom_magnification_end = zoom;
}

void Camera::setZoomAdd(double zoom_add, int count) {
	this->zoom_count = 0;
	this->zoom_count_max = count;

	this->zoom_magnification_start = this->zoom_magnification_now;
	this->zoom_magnification_end = this->zoom_magnification_now + zoom_add;
}

void Camera::setZoomReset(int count) {
	this->zoom_count = 0;
	this->zoom_count_max = count;

	this->zoom_magnification_start = this->zoom_magnification_now;
	this->zoom_magnification_end = 1.0;
}

void Camera::setRotation(double angle) {
	this->rotate_count = 0;
	this->rotate_count_max = 0;

	this->rotate_angle_now = angle;
	this->rotate_angle_start = angle;
	this->rotate_angle_end = angle;
}

void Camera::setRotation(double angle, int count) {
	this->rotate_count = 0;
	this->rotate_count_max = count;

	this->rotate_angle_start = this->rotate_angle_now;
	this->rotate_angle_end = angle;
}

void Camera::setRotationAdd(double angle_add, int count) {
	this->rotate_count = 0;
	this->rotate_count_max = count;

	this->rotate_angle_start = this->rotate_angle_now;
	this->rotate_angle_end = this->rotate_angle_now + angle_add;
}

void Camera::setSwayRandomInSquare(int pow, int count_max, int update_time) {
	this->sway_count = 0;
	this->sway_count_max = count_max;
	this->sway_power_max = pow;
	this->sway_update_time = update_time;

	this->sway_now = Vec2D(0.0, 0.0);
	this->sway_vib_rotate_freq = 0;
	this->sway_vib_trans_rad = 0.0;
}

void Camera::setSwayVibrationDecaying(int pow, int count_max, double trans_rad, int rot_freq) {
	this->sway_count = 0;
	this->sway_count_max = count_max;
	this->sway_power_max = pow;
	this->sway_vib_rotate_freq = rot_freq;
	this->sway_vib_trans_rad = trans_rad;

	this->sway_now = Vec2D(0.0, 0.0);
	this->sway_update_time = 0;
}


void Camera::drawImageInWorld(Vec2D center_pos, int graphHandle, const std::shared_ptr<ImagePropertyData> &img_prop) {
	Vec2D p = this->toWindowPosPxFromWorldPos(center_pos) + this->sway_now;
	double d = this->zoom_magnification_now*img_prop->img_expansion;
	double a = img_prop->img_rotation - this->rotate_angle_now;

	if (!this->isVisibleForWindowPos(p, img_prop->img_size*d, a)) return;

	SetDrawBright((int)(this->shield_ratio * 255), (int)(this->shield_ratio * 255), (int)(this->shield_ratio * 255));
	DrawRotaGraph2((int)p.x, (int)p.y, img_prop->img_size.width / 2, img_prop->img_size.height / 2, d, a, graphHandle, true, img_prop->img_turn);
}

void Camera::drawImageInWorld(Vec2D center_pos, Vec2D img_center, int graphHandle, const std::shared_ptr<ImagePropertyData> &img_prop) {
	Vec2D p = this->toWindowPosPxFromWorldPos(center_pos) + this->sway_now;
	double d = this->zoom_magnification_now*img_prop->img_expansion;
	double a = img_prop->img_rotation - this->rotate_angle_now;

	if (!this->isVisibleForWindowPos(p, img_prop->img_size*d, a)) return;

	SetDrawBright((int)(this->shield_ratio * 255), (int)(this->shield_ratio * 255), (int)(this->shield_ratio * 255));
	DrawRotaGraph2((int)p.x, (int)p.y, (int)img_center.x, (int)img_center.y, d, a, graphHandle, true, img_prop->img_turn);
}

void Camera::drawImageOnWindow(Vec2D center_pos, int graphHandle, const std::shared_ptr<ImagePropertyData> &img_prop) {
	Vec2D p = center_pos + this->sway_now;

	if (!this->isVisibleForWindowPos(center_pos, img_prop->img_size*img_prop->img_expansion)) return;

	SetDrawBright((int)(this->shield_ratio * 255), (int)(this->shield_ratio * 255), (int)(this->shield_ratio * 255));
	DrawRotaGraph2((int)p.x, (int)p.y, img_prop->img_size.width / 2, img_prop->img_size.height / 2, img_prop->img_expansion, img_prop->img_rotation, graphHandle, true, img_prop->img_turn);
}

void Camera::drawImageOnWindow(Vec2D center_pos, Vec2D img_center, int graphHandle, const std::shared_ptr<ImagePropertyData> &img_prop) {
	Vec2D p = center_pos + this->sway_now;

	if (!this->isVisibleForWindowPos(center_pos, img_prop->img_size*img_prop->img_expansion)) return;

	SetDrawBright((int)(this->shield_ratio * 255), (int)(this->shield_ratio * 255), (int)(this->shield_ratio * 255));
	DrawRotaGraph2((int)p.x, (int)p.y, (int)img_center.x, (int)img_center.y, img_prop->img_expansion, img_prop->img_rotation, graphHandle, true, img_prop->img_turn);
}


void Camera::drawSquareOnWindowFixed(Vec2D pos_center, Size size, Color_RGB color, bool fill) {
	if (!this->isVisibleForWindowPos(pos_center, size)) return;

	Vec2D lu = pos_center - (size / 2).toVec();
	Vec2D rd = pos_center + (size / 2).toVec();
	Color_RGB c = color*this->shield_ratio;

	DrawBox((int)lu.x, (int)lu.y, (int)rd.x, (int)rd.y, c.toColor(), fill);
}

void Camera::drawSquareOnWindowSwayed(Vec2D pos_center, Size size, Color_RGB color, bool fill) {
	if (!this->isVisibleForWindowPos(pos_center + this->sway_now, size)) return;

	Vec2D lu = pos_center - (size / 2).toVec() + this->sway_now;
	Vec2D rd = pos_center + (size / 2).toVec() + this->sway_now;
	Color_RGB c = color*this->shield_ratio;

	DrawBox((int)lu.x, (int)lu.y, (int)rd.x, (int)rd.y, c.toColor(), fill);
}

void Camera::drawRotateSquareOnWindowFixed(Vec2D pos_center, Size size, double rot_angle, Color_RGB color, bool fill) {
	if (!this->isVisibleForWindowPos(pos_center, size, rot_angle)) return;

	Vec2D p1 = pos_center + Mat2D::rotation(rot_angle, Vec2D(-size.width / 2, -size.height / 2));
	Vec2D p2 = pos_center + Mat2D::rotation(rot_angle, Vec2D(size.width / 2, -size.height / 2));
	Vec2D p3 = pos_center + Mat2D::rotation(rot_angle, Vec2D(size.width / 2, size.height / 2));
	Vec2D p4 = pos_center + Mat2D::rotation(rot_angle, Vec2D(-size.width / 2, size.height / 2));
	Color_RGB c = color*this->shield_ratio;

	DrawQuadrangle((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, (int)p3.x, (int)p3.y, (int)p4.x, (int)p4.y, c.toColor(), fill);
}

void Camera::drawRotateSquareOnWindowSwayed(Vec2D pos_center, Size size, double rot_angle, Color_RGB color, bool fill) {
	if (!this->isVisibleForWindowPos(pos_center + this->sway_now, size, rot_angle)) return;

	Vec2D p1 = pos_center + Mat2D::rotation(rot_angle, Vec2D(-size.width / 2, -size.height / 2)) + this->sway_now;
	Vec2D p2 = pos_center + Mat2D::rotation(rot_angle, Vec2D(size.width / 2, -size.height / 2)) + this->sway_now;
	Vec2D p3 = pos_center + Mat2D::rotation(rot_angle, Vec2D(size.width / 2, size.height / 2)) + this->sway_now;
	Vec2D p4 = pos_center + Mat2D::rotation(rot_angle, Vec2D(-size.width / 2, size.height / 2)) + this->sway_now;
	Color_RGB c = color*this->shield_ratio;

	DrawQuadrangle((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, (int)p3.x, (int)p3.y, (int)p4.x, (int)p4.y, c.toColor(), fill);
}

void Camera::drawRotateSquareInWorld(Vec2D pos_center, Size original_size, double rot_angle, Color_RGB color, bool fill) {
	Vec2D p = this->toWindowPosPxFromWorldPos(pos_center) + this->sway_now;
	Size s = original_size * this->zoom_magnification_now;
	double a = rot_angle - this->rotate_angle_now;

	if (!this->isVisibleForWindowPos(p, s, a)) return;

	Vec2D p1 = p + Mat2D::rotation(a, Vec2D(-s.width / 2, -s.height / 2));
	Vec2D p2 = p + Mat2D::rotation(a, Vec2D( s.width / 2, -s.height / 2));
	Vec2D p3 = p + Mat2D::rotation(a, Vec2D( s.width / 2,  s.height / 2));
	Vec2D p4 = p + Mat2D::rotation(a, Vec2D(-s.width / 2,  s.height / 2));
	Color_RGB c = color*this->shield_ratio;

	DrawQuadrangle((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, (int)p3.x, (int)p3.y, (int)p4.x, (int)p4.y, c.toColor(), fill);
}

void Camera::drawLineInWorld(Vec2D pos_start, Vec2D pos_end, Color_RGB color, int width) {
	Vec2D s = this->toWindowPosPxFromWorldPos(pos_start) + this->sway_now;
	Vec2D e = this->toWindowPosPxFromWorldPos(pos_end) + this->sway_now;

	if ((!this->isVisibleForWindowPos(s)) && (!this->isVisibleForWindowPos(e))) return;

	Color_RGB c = color*this->shield_ratio;
	DrawLine((int)s.x, (int)s.y, (int)e.x, (int)e.y, c.toColor(), width);
}

void Camera::drawLineOnWindowFixed(Vec2D pos_start, Vec2D pos_end, Color_RGB color, int width) {
	if ((!this->isVisibleForWindowPos(pos_start)) && (!this->isVisibleForWindowPos(pos_end))) return;

	Color_RGB c = color*this->shield_ratio;
	DrawLine((int)pos_start.x, (int)pos_start.y, (int)pos_end.x, (int)pos_end.y, c.toColor(), width);
}

void Camera::drawLineOnWindowSwayed(Vec2D pos_start, Vec2D pos_end, Color_RGB color, int width) {
	Vec2D s = pos_start + this->sway_now;
	Vec2D e = pos_end + this->sway_now;

	if ((!this->isVisibleForWindowPos(s)) && (!this->isVisibleForWindowPos(e))) return;

	Color_RGB c = color*this->shield_ratio;
	DrawLine((int)s.x, (int)s.y, (int)e.x, (int)e.y, c.toColor(), width);
}

void Camera::drawCircleInWorld(Vec2D pos_center, int r_original, Color_RGB color, bool fill) {
	Vec2D c = this->toWindowPosPxFromWorldPos(pos_center) + this->sway_now;

	if (!this->isVisibleForWindowPos(c, Size(2 * r_original, 2 * r_original)*this->zoom_magnification_now)) return;

	Color_RGB col = color*this->shield_ratio;
	DrawCircle((int)c.x, (int)c.y, (int)(r_original*this->zoom_magnification_now), col.toColor(), fill);
}

void Camera::drawCircleOnWindowFixed(Vec2D pos_center, int r, Color_RGB color, bool fill) {
	if (!this->isVisibleForWindowPos(pos_center, Size(2 * r, 2 * r))) return;

	Color_RGB col = color*this->shield_ratio;
	DrawCircle((int)pos_center.x, (int)pos_center.y, r, col.toColor(), fill);
}

void Camera::drawCircleOnWindowSwayed(Vec2D pos_center, int r, Color_RGB color, bool fill) {
	Vec2D c = pos_center + this->sway_now;

	if (!this->isVisibleForWindowPos(c, Size(2 * r, 2 * r))) return;

	Color_RGB col = color*this->shield_ratio;
	DrawCircle((int)c.x, (int)c.y, r, col.toColor(), fill);
}

void Camera::drawStringInWorld(Vec2D pos_center, std::string text, Color_RGB color, double expansion) {
	Vec2D p = this->toWindowPosPxFromWorldPos(pos_center) + this->sway_now;
	double d = expansion*this->zoom_magnification_now;
	Size s = Size(GetDrawStringWidth(text.c_str(), text.length()), Resources::getFont()->font_size)*d;
	double a = -this->rotate_angle_now;

	if (!this->isVisibleForWindowPos(p, s, a)) return;
	
	Color_RGB c = color*this->shield_ratio;
	DrawRotaString((int)p.x, (int)p.y, d, d, s.width / 2, s.height / 2, a, c.toColor(), 0, 0, text.c_str());
}

void Camera::drawStringOnWindowFixed(Vec2D pos_center, std::string text, Color_RGB color, double expansion) {
	Size s = Size(GetDrawStringWidth(text.c_str(), text.length()), Resources::getFont()->font_size)*expansion;

	if (!this->isVisibleForWindowPos(pos_center, s)) return;

	Color_RGB c = color*this->shield_ratio;
	DrawRotaString((int)pos_center.x, (int)pos_center.y, expansion, expansion, s.width / 2, s.height / 2, 0.0, c.toColor(), 0, 0, text.c_str());
}

void Camera::drawStringOnWindowSwayed(Vec2D pos_center, std::string text, Color_RGB color, double expansion) {
	Vec2D p = pos_center + this->sway_now;
	Size s = Size(GetDrawStringWidth(text.c_str(), text.length()), Resources::getFont()->font_size)*expansion;

	if (!this->isVisibleForWindowPos(p, s)) return;

	Color_RGB c = color*this->shield_ratio;
	DrawRotaString((int)p.x, (int)p.y, expansion, expansion, s.width / 2, s.height / 2, 0.0, c.toColor(), 0, 0, text.c_str());
}



void Camera::update(){
	if (this->move_count < this->move_count_max) {
		this->move_count++;
		this->pos_anchor_world_now = this->anchor_target_world_pos_start + (this->anchor_target_world_pos_goal - this->anchor_target_world_pos_start)*(1.0*this->move_count / this->move_count_max);
	}
	else if (this->move_count == this->move_count_max) {
		this->pos_anchor_world_now = this->anchor_target_world_pos_goal;
	}
	if (this->pos_anchor_world_now.x < this->pos_anchor_window.x) this->pos_anchor_world_now.x = this->pos_anchor_window.x;
	if (this->pos_anchor_world_now.y < this->pos_anchor_window.y) this->pos_anchor_world_now.y = this->pos_anchor_window.y;
	if (this->pos_anchor_world_now.x > this->worldSize.width  - this->pos_anchor_window.x) this->pos_anchor_world_now.x = this->worldSize.width  - this->pos_anchor_window.x;
	if (this->pos_anchor_world_now.y > this->worldSize.height - this->pos_anchor_window.y) this->pos_anchor_world_now.y = this->worldSize.height - this->pos_anchor_window.y;

	if (this->zoom_count < this->zoom_count_max) {
		this->zoom_count++;
		this->zoom_magnification_now = this->zoom_magnification_start + (this->zoom_magnification_end - this->zoom_magnification_start)*this->zoom_count / this->zoom_count_max;
	}
	else if (this->zoom_count == this->zoom_count_max) {
		this->zoom_magnification_now = this->zoom_magnification_end;
	}
	
	if (this->rotate_count < this->rotate_count_max) {
		this->rotate_count++;
		this->rotate_angle_now = this->rotate_angle_start + (this->rotate_angle_end - this->rotate_angle_start)*this->rotate_count / this->rotate_count_max;
	}
	else if (this->rotate_count == this->rotate_count_max) {
		this->rotate_angle_now = this->rotate_angle_end;
	}

	if (this->sway_count < this->sway_count_max) {
		this->sway_count++;
		if (this->sway_update_time == 0) {
			double p = M_PI*2*this->sway_count / this->sway_vib_rotate_freq;
			this->sway_now = Vec2D::fromPowAng(1.0*this->sway_power_max*(this->sway_count_max - this->sway_count) / this->sway_count_max*sin(p), this->sway_vib_trans_rad);
		}
		else {
			if (this->sway_count%this->sway_update_time == 0) {
				this->sway_now = Vec2D::getRandomInSquare(this->sway_power_max, this->sway_power_max);
			}
		}
	}
	else {
		this->sway_now = Vec2D(0.0, 0.0);
	}
}



bool Camera::isVisibleForWindowPos(Vec2D p) {
	if (p.x < 0.0) return false;
	if (p.y < 0.0) return false;
	if (p.x > this->windowPixelSize.width) return false;
	if (p.y > this->windowPixelSize.height) return false;

	return true;
}

bool Camera::isVisibleForWindowPos(Vec2D p, Size s) {
	if (p.x < -s.width / 2) return false;
	if (p.y < -s.height / 2) return false;
	if (p.x > this->windowPixelSize.width + s.width / 2) return false;
	if (p.y > this->windowPixelSize.height + s.height / 2) return false;

	return true;
}

bool Camera::isVisibleForWindowPos(Vec2D p, Size s, double ang) {
	Vec2D p1 = p + Mat2D::rotation(ang, Vec2D(-s.width / 2, -s.height / 2));
	Vec2D p2 = p + Mat2D::rotation(ang, Vec2D( s.width / 2, -s.height / 2));
	Vec2D p3 = p + Mat2D::rotation(ang, Vec2D(-s.width / 2,  s.height / 2));
	Vec2D p4 = p + Mat2D::rotation(ang, Vec2D( s.width / 2,  s.height / 2));

	Vec2D w1 = Vec2D(0.0, 0.0);
	Vec2D w2 = Vec2D(this->windowPixelSize.width, 0.0);
	Vec2D w3 = Vec2D(this->windowPixelSize.width, this->windowPixelSize.height);
	Vec2D w4 = Vec2D(0.0, this->windowPixelSize.height);

	if (Vec2D::isCrossLine(p1, p2, w1, w2)) return true;
	if (Vec2D::isCrossLine(p1, p2, w2, w3)) return true;
	if (Vec2D::isCrossLine(p1, p2, w3, w4)) return true;
	if (Vec2D::isCrossLine(p1, p2, w4, w1)) return true;
	if (Vec2D::isCrossLine(p2, p3, w1, w2)) return true;
	if (Vec2D::isCrossLine(p2, p3, w2, w3)) return true;
	if (Vec2D::isCrossLine(p2, p3, w3, w4)) return true;
	if (Vec2D::isCrossLine(p2, p3, w4, w1)) return true;
	if (Vec2D::isCrossLine(p3, p4, w1, w2)) return true;
	if (Vec2D::isCrossLine(p3, p4, w2, w3)) return true;
	if (Vec2D::isCrossLine(p3, p4, w3, w4)) return true;
	if (Vec2D::isCrossLine(p3, p4, w4, w1)) return true;
	if (Vec2D::isCrossLine(p4, p1, w1, w2)) return true;
	if (Vec2D::isCrossLine(p4, p1, w2, w3)) return true;
	if (Vec2D::isCrossLine(p4, p1, w3, w4)) return true;
	if (Vec2D::isCrossLine(p4, p1, w4, w1)) return true;

	if (!Vec2D::isPointWholeInSquare(w1, w2, w3, w4, p1)) return false;
	if (!Vec2D::isPointWholeInSquare(w1, w2, w3, w4, p2)) return false;
	if (!Vec2D::isPointWholeInSquare(w1, w2, w3, w4, p3)) return false;
	if (!Vec2D::isPointWholeInSquare(w1, w2, w3, w4, p4)) return false;

	return true;
}