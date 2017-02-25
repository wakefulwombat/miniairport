#pragma once

#include <memory>
#include "common.h"
#include "base_externalObjectController.h"
#include "base_internalObjectController.h"
#include "camera.h"
#include "DxLib.h"
#include <math.h>
#include <vector>

enum class ControlStatus{
	None,//制御なし
	InternalControlled,//自律移動もしくは内部自動制御
	ExternalControlled//自律移動不可、外部制御(制限解除は外部が行う)
};

class CameraDrawInterface;

class ObjectManagementBaseKit : public RequiredFunc {
protected:
	bool validation;
	unsigned int z_sort;//zソート

public:
	bool getValidation() { return this->validation; }
	unsigned int getZSort() { return this->z_sort; }
	virtual void draw(const std::shared_ptr<CameraDrawInterface> &camera) const = 0;
};

//あらゆるオブジェクトの基底
class ObjectBase : private ExternalMinimalController, public ObjectManagementBaseKit {
private:
	//外部操作用
	void setWorldPosition(Vec2D pos) override final { this->world_pos = pos; }
	void setInvalid() override final { this->validation = false; }
	void setControlRights(ControlStatus status) override { this->control_status = status; }

protected:
	Vec2D world_pos;

	ControlStatus control_status;//制御権
	std::shared_ptr<InternalObjectController> internalController;//内部制御用

public:
	ObjectBase(Vec2D world_pos, int z_sort, std::shared_ptr<InternalObjectController> internalController) { this->world_pos = world_pos; this->validation = true; this->z_sort = z_sort; this->internalController = internalController; this->control_status = ControlStatus::InternalControlled; }
	virtual ~ObjectBase(){}

	Vec2D getWorldPosition() override final { return this->world_pos; }
	bool getValidation() override { return this->validation; }
};

//オブジェクトの集合体
class ObjectsBase : private ExternalMinimalController, public ObjectManagementBaseKit {
private:
	//外部操作用
	void setInvalid() override final { this->validation = false; }
	void setControlRights(ControlStatus status) override final { this->control_status = status; }

protected:
	std::vector<std::shared_ptr<ObjectBase>> components;

	ControlStatus control_status;//制御権
	std::shared_ptr<InternalObjectController> internalController;//内部制御用

public:
	ObjectsBase(Vec2D world_pos, int z_sort, std::shared_ptr<InternalObjectController> internalController) { this->validation = true; this->z_sort = z_sort; this->internalController = internalController; this->control_status = ControlStatus::InternalControlled; }
	void addComponent(const std::shared_ptr<ObjectBase> &obj) { this->components.push_back(obj); }
	void update() override { for (auto it = this->components.begin(); it != this->components.end();) { if (!(*it)->getValidation()) { it = this->components.erase(it); } else { (*it)->update(); ++it; } } }
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override { for (auto it = this->components.begin(); it != this->components.end();) (*it)->draw(camera); }
	virtual ~ObjectsBase() {}
};



//画像を使うオブジェクト
class ImagePropertyData {
public:
	const Size img_size;//表示する画像の元サイズ
	double img_expansion;//画像拡大率
	double img_rotation;//画像回転角度
	double img_opacity;//画像透明度(0.0~1.0)
	bool img_turn;//左右反転

	ImagePropertyData(Size img_size) :img_size(img_size) {
		this->img_expansion = 1.0;
		this->img_rotation = 0.0;
		this->img_opacity = 1.0;
		this->img_turn = false;
	}
};

class ImageProperty : private ExternalImageController {
private:
	void setImageRotation(double rad) override final { this->img_prop->img_rotation = rad; }
	void addImageRotation(double addRad) override final { this->img_prop->img_rotation += addRad; }
	void setImageOpacity(double opacity) override final { this->img_prop->img_opacity = opacity; }//0.0~1.0
	void setObjectSizeScalingFromNow(double magnification, bool AddOrTime = true) override final { if (AddOrTime) this->img_prop->img_expansion += magnification; else this->img_prop->img_expansion *= magnification; }

protected:
	//for image
	std::shared_ptr<ImagePropertyData> img_prop;

public:
	ImageProperty(std::shared_ptr<ImagePropertyData> img_prop) : img_prop(img_prop) {}
	~ImageProperty() {}

	Size getObjectSize() override final { return this->img_prop->img_size*this->img_prop->img_expansion; }//実際に表示するサイズ
};



//移動するオブジェクト
class MovePropertyData {
public:
	//直進運動
	double trans_vel, trans_acc, trans_angle;
	
	//回転運動
	Vec2D rotate_center_world_pos;
	double rotate_radius, rotate_angle, rotate_omega;

	MovePropertyData() {
		this->trans_vel = 0.0;
		this->trans_angle = 0.0;
		this->rotate_angle = 0.0;
		this->rotate_center_world_pos = Vec2D(0.0, 0.0);
		this->rotate_omega = 0.0;
		this->rotate_radius = 0.0;
	}

	MovePropertyData(double vel, double angle) {
		this->trans_vel = vel;
		this->trans_angle = angle;
		this->rotate_angle = 0.0;
		this->rotate_center_world_pos = Vec2D(0.0, 0.0);
		this->rotate_omega = 0.0;
		this->rotate_radius = 0.0;
	}

	MovePropertyData(Vec2D rotate_center_world_pos, double rotate_radius, double rotate_angle, double rotate_omega) {
		this->trans_vel = 0.0;
		this->trans_angle = 0.0;
		this->rotate_angle = rotate_angle;
		this->rotate_center_world_pos = rotate_center_world_pos;
		this->rotate_omega = rotate_omega;
		this->rotate_radius = rotate_radius;
	}
};

class MoveProperty : private ExternalMoveController{
private:
	//外部操作用
	void setTransVelocityVector(Vec2D vec) override final { this->mv_prop->trans_vel = vec.toNorm(); this->mv_prop->trans_angle = vec.toAngle(); }
	void setTransVelocityNorm(double vel) override final { this->mv_prop->trans_vel = vel; }
	void setTransAccelerationVector(Vec2D vec) override final { this->mv_prop->trans_acc = vec.toNorm(); this->mv_prop->trans_angle = vec.toAngle(); }
	void setTransAccelerationNorm(double acc) override final { this->mv_prop->trans_acc = acc; }
	void setTransAngle(double angle) override final { this->mv_prop->trans_angle = angle; }

	void setRotateCenterWorldPos(Vec2D pos) override final { this->mv_prop->rotate_center_world_pos = pos; }
	void setRotateRadius(double radius) override final { this->mv_prop->rotate_radius = radius; }
	void setRotateAngle(double angle) override final { this->mv_prop->rotate_angle = angle; }
	void setRotateOmega(double omega) override final { this->mv_prop->rotate_omega = omega; }

protected:
	std::shared_ptr<MovePropertyData> mv_prop;

public:
	MoveProperty(std::shared_ptr<MovePropertyData> mv_prop) { this->mv_prop = mv_prop; }
	~MoveProperty() {}

	double getTransVelocityNorm() override final { return this->mv_prop->trans_vel; }
	Vec2D getTransVelocityVector() override final { return Vec2D::fromPowAng(this->mv_prop->trans_vel, this->mv_prop->trans_angle); }
	double getTransAccelerationNorm() override final { return this->mv_prop->trans_acc; }
	Vec2D getTransAccelerationVector() override final { return Vec2D::fromPowAng(this->mv_prop->trans_acc, this->mv_prop->trans_angle); }
	double getTransAngle() override final { return this->mv_prop->trans_angle; }

	Vec2D getRotateCenterWorldPosition() override final { return this->mv_prop->rotate_center_world_pos; }
	double getRotateRadius() override final { return this->mv_prop->rotate_radius; }
	double getRotateAngle() override final { return this->mv_prop->rotate_angle; }
	double getRotateOmega() override final { return this->mv_prop->rotate_omega; }
};