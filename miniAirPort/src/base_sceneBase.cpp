#include "base_sceneBase.h"

Layer_Pause::Layer_Pause(const Size window_size) : LayerBase(0.5, false, std::make_shared<Camera>(window_size)) {
	this->back = std::make_shared<DisappearButton_Fix>(Vec2D(480, 360), Size(250, 100), "ゲームに戻る", 32, [this]() {this->setInvalid(); });
	this->title = std::make_shared<DisappearButton_Fix>(Vec2D(800, 360), Size(250, 100), "タイトルに戻る", 32, []() {});

	this->addObject(this->back);
	this->addObject(this->title);
}

void Layer_Pause::initialize() {

}

void Layer_Pause::update() {

}



Layer_GameOver::Layer_GameOver(const Size window_size) : LayerBase(0.3, true, std::make_shared<Camera>(window_size)) {

}

void Layer_GameOver::initialize() {

}

void Layer_GameOver::update() {

}