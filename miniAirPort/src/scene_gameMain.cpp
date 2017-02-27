#include "scene_gameMain.h"
#include "world.h"

SceneGameMain::SceneGameMain(const std::shared_ptr<SceneCommonData> &common) : SceneBase(common) {
	this->commonData = common;
	//this->initialize();
}

void SceneGameMain::initialize() {
	SceneBase::initialize();
	this->layers.push_back(std::make_shared<World>(this->commonData->stage_num));
}

void SceneGameMain::update() {
	SceneBase::update();
}