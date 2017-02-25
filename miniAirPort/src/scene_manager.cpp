#include "scene_manager.h"
#include "scene_gameMain.h"
#include "base_sceneBase.h"
#include <deque>

SceneManager::SceneManager(Size screenSize) : SceneCommonData(screenSize) {
	this->initialize();
}

void SceneManager::initialize() {
	this->newSceneKind = SceneKindAll::GameMain;
}

void SceneManager::update() {
	if (this->newSceneKind != SceneKindAll::None) {
		if (this->nowScene != nullptr) {
			this->nowScene->finalize();
		}

		switch (this->newSceneKind) {
		case SceneKindAll::Ending:
			this->nowScene = nullptr;
			break;

		case SceneKindAll::GameMain:
			this->nowScene = std::make_shared<SceneGameMain>(this->shared_from_this());
			break;

		case SceneKindAll::StageSelect:
			this->nowScene = nullptr;
			break;

		case SceneKindAll::Result:
			this->nowScene = nullptr;
			break;

		case SceneKindAll::Title:
			this->nowScene = nullptr;
			break;
		}

		this->newSceneKind = SceneKindAll::None;
		this->nowScene->initialize();
	}
	else {
		this->nowScene->update();
	}
}

void SceneBase::update() {
	if (!this->layers[this->layers.size() - 1]->getValidation()) this->layers.pop_back();

	std::deque<double> shields;
	std::deque<bool> update_allowed;

	shields.push_front(1.0);
	update_allowed.push_front(true);
	for (int i = this->layers.size() - 2; i >= 0; --i) { shields.push_front(this->layers[i + 1]->getShieldRatioOfUnderLayer()*shields.front()); }
	for (int i = this->layers.size() - 2; i >= 0; --i) {
		if (!this->layers[i + 1]->doesAllowedUpdateUnderLayer()) {
			for (int j = i; j >= 0; --j) update_allowed.push_front(false);
			break;
		}
	}

	this->layers[this->layers.size() - 1]->addObject(this->commonData->mouse_pointer);
	this->commonData->mouse_pointer_world_pos = this->layers[this->layers.size() - 1]->getCamera()->toWorldPosFromWindowPosPx(this->commonData->mouse_pointer->getWorldPosition());

	for (unsigned int i = 0; i < this->layers.size(); ++i) {
		if (update_allowed[i]) this->layers[i]->update();
		this->layers[i]->setCameraShieldRatio(shields[i]);
		this->layers[i]->draw();
	}

	this->layers[this->layers.size() - 1]->popBackObject();
}