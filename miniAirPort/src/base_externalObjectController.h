#pragma once

#include "common.h"

enum class ControlStatus;

class ExternalControllerRequired {
private:
	bool end;

public:
	ExternalControllerRequired() { this->end = false; }
	~ExternalControllerRequired() {}
	bool isEnd() { return this->end; }
	virtual void setControlRights(ControlStatus status) = 0;
};

class ExternalMinimalController : public ExternalControllerRequired {
public:
	virtual ~ExternalMinimalController() {}

	virtual Vec2D getWorldPosition() = 0;
	virtual bool getValidation() = 0;

	virtual void setWorldPosition(Vec2D pos) = 0;
	virtual void setInvalid() = 0;
};

class ExternalImageController : public ExternalControllerRequired {
public:
	virtual ~ExternalImageController() {}
	virtual void setImageRotation(double rad) = 0;
	virtual void addImageRotation(double addRad) = 0;
	virtual void setImageOpacity(double opacity) = 0;
	virtual void setObjectSizeScalingFromNow(double magnification, bool AddOrTime = true) = 0;

	virtual Size getObjectSize() = 0;
};

class ExternalMoveController : public ExternalControllerRequired {
public:
	virtual ~ExternalMoveController() {}

	virtual double getTransVelocityNorm() = 0;
	virtual Vec2D getTransVelocityVector() = 0;
	virtual double getTransAccelerationNorm() = 0;
	virtual Vec2D getTransAccelerationVector() = 0;
	virtual double getTransAngle() = 0;

	virtual Vec2D getRotateCenterWorldPosition() = 0;
	virtual double getRotateRadius() = 0;
	virtual double getRotateAngle() = 0;
	virtual double getRotateOmega() = 0;
	

	virtual void setTransVelocityVector(Vec2D vec) = 0;
	virtual void setTransVelocityNorm(double vel) = 0;
	virtual void setTransAccelerationVector(Vec2D vec) = 0;
	virtual void setTransAccelerationNorm(double acc) = 0;
	virtual void setTransAngle(double angle) = 0;

	virtual void setRotateCenterWorldPos(Vec2D pos) = 0;
	virtual void setRotateRadius(double radius) = 0;
	virtual void setRotateAngle(double angle) = 0;
	virtual void setRotateOmega(double omega) = 0;
};