#pragma once

#include "base_sceneBase.h"

enum class SceneKindAll {
	None,
	Title,
	StageSelect,
	GameMain,
	Result,
	Ending
};

class SceneManager : public RequiredFunc, public SceneCommonData, public std::enable_shared_from_this<SceneManager> {
private:
	SceneKindAll newSceneKind;
	std::shared_ptr<SceneBase> nowScene;

public:
	SceneManager(Size window_size);
	void initialize() override;
	void update() override;

	Size getWindowSize() { return this->window_size; }
};