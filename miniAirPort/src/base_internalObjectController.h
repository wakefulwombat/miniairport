#pragma once

#include "common.h"
#include <memory>

class InternalObjectController{
public:
	virtual ~InternalObjectController(){}
	virtual void update() = 0;
	virtual bool isEnd() { return true; }
};

class InternalObjectController_None : public InternalObjectController {
public:
	InternalObjectController_None() {}
	void update() override {}
};

class InternalObjectControllerFactory {
public:
	static std::shared_ptr<InternalObjectController_None> getInternalController_None() { return std::make_shared<InternalObjectController_None>(); }
};