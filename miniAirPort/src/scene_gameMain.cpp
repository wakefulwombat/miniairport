#include "scene_gameMain.h"
#include "world.h"
#include "input.h"

SceneGameMain::SceneGameMain(const std::shared_ptr<SceneCommonData> &common) : SceneBase(common) {
	this->commonData = common;
	//this->initialize();
}

void SceneGameMain::initialize() {
	SceneBase::initialize();
	this->layers.push_back(std::make_shared<World>(this->commonData->stage_num, [this](const std::shared_ptr<LayerBase>& layer) {this->pushLayer(layer); }));
}

void SceneGameMain::update() {
	SceneBase::update();
	this->checkEvent(Input_T::getOperationInterface_mouse()->getPointerPosition());
}