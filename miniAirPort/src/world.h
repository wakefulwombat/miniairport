#pragma once
#include "common.h"
#include <vector>
#include <memory>
#include "base_sceneBase.h"
#include "base_objectBase.h"
#include "resource.h"

class MapChip : public ObjectBase, public ImageProperty {
private:
	const int kind;
	const bool can_pass;

public:
	MapChip(Vec2D center, Size size, int kind, bool can_pass) : ObjectBase(center, 1, std::make_shared<InternalObjectController_None>()), ImageProperty(std::make_shared<ImagePropertyData>(size)), kind(kind), can_pass(can_pass) { this->initialize(); }
	void initialize() override {}
	void update() override {}
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override { if (!this->validation) return; camera->drawImageInWorld(this->world_pos, Resources::getImage()->getMapChip(this->kind), this->img_prop); }

	void setControlRights(ControlStatus status) override final { this->control_status = status; }
};

class World : public LayerBase {
private:
	Size chipSize_pixel;
	Size worldSize_chipNum;

	void loadStageInfo(int stage);
	void loadStageChip(int stage);

public:
	World(int stage) : LayerBase(0.0, false, std::make_shared<Camera>(Size(1280, 720))) { this->initialize(); this->loadStageInfo(stage); this->loadStageChip(stage); }
	void initialize() override {}
	Size getWorldSizePixel() { return Size(this->chipSize_pixel.width*this->worldSize_chipNum.width, this->chipSize_pixel.height*this->worldSize_chipNum.height); }
};