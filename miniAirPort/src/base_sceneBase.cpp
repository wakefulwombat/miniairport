#include "base_sceneBase.h"

Layer_Pause::Layer_Pause(const Size window_size) : LayerBase(0.5, false, std::make_shared<Camera>(window_size)) {

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