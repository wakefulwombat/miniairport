#pragma once

#include "base_sceneBase.h"

class SceneGameMain : public SceneBase {
private:

public:
	SceneGameMain(const std::shared_ptr<SceneCommonData> &common);
	void initialize() override;
	void update() override;
};
