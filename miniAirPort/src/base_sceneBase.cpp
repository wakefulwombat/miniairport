#include "base_sceneBase.h"
#include <deque>

void SceneBase::update() {
	if (!this->layers[this->layers.size() - 1]->getValidation()) this->layers.pop_back();
	unsigned int l = this->layers.size();

	std::deque<double> shields;
	std::deque<bool> update_allowed;

	shields.push_front(1.0);
	update_allowed.push_front(true);
	for (int i = l - 2; i >= 0; --i) { shields.push_front(this->layers[i + 1]->getShieldRatioOfUnderLayer()*shields.front()); }
	for (int i = l - 2; i >= 0; --i) {
		if (!this->layers[i + 1]->doesAllowedUpdateUnderLayer()) {
			for (int j = i; j >= 0; --j) update_allowed.push_front(false);
			break;
		}
		else update_allowed.push_front(true);
	}

	this->layers[l - 1]->addObject(this->commonData->mouse_pointer);
	this->commonData->mouse_pointer_world_pos = this->layers[l - 1]->getCamera()->toWorldPosFromWindowPosPx(this->commonData->mouse_pointer->getWorldPosition());

	
	for (unsigned int i = 0; i < l; ++i) {
		if (update_allowed[i]) this->layers[i]->update();
		this->layers[i]->setCameraShieldRatio(shields[i]);
		this->layers[i]->draw();
	}

	this->layers[l - 1]->popBackObject();
}

Layer_Pause::Layer_Pause(const Size window_size) : LayerBase(0.5, false, std::make_shared<Camera>(window_size)) {
	this->back = std::make_shared<DisappearButton_Fix>(Vec2D(400, 360), Size(250, 100), "ゲームに戻る", 32, [this]() {this->setInvalid(); });
	this->title = std::make_shared<DisappearButton_Fix>(Vec2D(880, 360), Size(250, 100), "タイトルに戻る", 32, []() {});

	this->addObject(this->back);
	this->addObject(this->title);
}

void Layer_Pause::initialize() {

}

void Layer_Pause::update() {
	LayerBase::update();
}



Layer_GameOver::Layer_GameOver(const Size window_size) : LayerBase(0.3, true, std::make_shared<Camera>(window_size)) {

}

void Layer_GameOver::initialize() {

}

void Layer_GameOver::update() {

}