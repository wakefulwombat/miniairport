#pragma once
#include "common.h"
#include <vector>
#include <memory>
#include "base_sceneBase.h"
#include "base_objectBase.h"
#include "timeTable.h"
#include "resource.h"
#include "button.h"

class MapChip : public ObjectBase, public ImageProperty {
private:
	const int kind;
	const bool can_pass;

public:
	MapChip(Vec2D center, Size size, int kind, bool can_pass) : ObjectBase(center, 1), ImageProperty(std::make_shared<ImagePropertyData>(size)), kind(kind), can_pass(can_pass) { this->initialize(); }
	void initialize() override {}
	void update() override {}
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override { if (!this->validation) return; camera->drawImageInWorld(this->world_pos, Resources::getImage()->getMapChip(this->kind), this->img_prop); }

	void setControlRights(ControlStatus status) override final { this->control_status = status; }
};

class World : public LayerBase {
private:
	Size chipSize_pixel;
	Size worldSize_chipNum;
	
	const std::function<void(const std::shared_ptr<LayerBase>&)> push_layer;

	std::shared_ptr<RadioButton_Fix> high_speed;
	std::shared_ptr<IconRadioButton_Fix> show_arrivalTimeTable, show_departureTimeTable;

	std::shared_ptr<TimeTable> time_table;

	void loadStageInfo(int stage);
	void loadStageTimeTable(int stage);
	void loadStageChip(int stage);

public:
	World(int stage, std::function<void(const std::shared_ptr<LayerBase>&)> push_layer);
	void initialize() override { LayerBase::initialize(); }
	void update() override;

	Size getWorldSizePixel() { return Size(this->chipSize_pixel.width*this->worldSize_chipNum.width, this->chipSize_pixel.height*this->worldSize_chipNum.height); }
	bool isHighSpeedNow() { return this->high_speed->isOn(); }

	void offIconButtonArrival() { this->show_arrivalTimeTable->setOff(); }
	void offIconButtonDeparture() { this->show_departureTimeTable->setOff(); }
};