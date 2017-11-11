#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <future>
#include "base_objectBase.h"
#include "Camera.h"
#include "mousePointer.h"
#include "button.h"

class LayerBase : public RequiredFunc {
protected:
	std::shared_ptr<Camera> camera;

	std::vector<std::shared_ptr<ObjectBase>> objects;
	std::vector<std::shared_ptr<EventCheck>> click_checks;
	double under_layer_shield_ratio;//下層レイヤーの遮蔽率(描画前にカメラに設定する)(0.0で映さない)
	bool allow_update_under_layer;//下層レイヤーを更新するか
	bool allow_click_check_under_layer;//下層レイヤーへのクリック判定を許可するか
	bool validation;

	virtual void checkEvent_backGroundClicked(Vec2D mouse_window_pos) {};

public:
	LayerBase(double under_layer_shield_ratio, bool allow_update_under_layer, bool allow_click_check_under_layer, const std::shared_ptr<Camera> &camera) { this->under_layer_shield_ratio = under_layer_shield_ratio; this->allow_update_under_layer = allow_update_under_layer; this->allow_click_check_under_layer = allow_click_check_under_layer; this->camera = camera; this->initialize(); }
	virtual ~LayerBase() {}

	std::shared_ptr<Camera> getCamera() { return this->camera; }
	void setCameraShieldRatio(double ratio) { return this->camera->setShieldRatio(ratio); }
	bool getValidation() { return this->validation; }
	void addObject(const std::shared_ptr<ObjectBase> &obj) { this->objects.push_back(obj); std::sort(this->objects.begin(), this->objects.end(), [](const std::shared_ptr<ObjectManagementBaseKit> &left, const std::shared_ptr<ObjectManagementBaseKit> &right) {return left->getZSort() < right->getZSort(); }); }
	void addEventCheck(const std::shared_ptr<ObjectBase> &obj) { this->click_checks.push_back(obj); std::sort(this->click_checks.begin(), this->click_checks.end(), [](const std::shared_ptr<EventCheck> &left, const std::shared_ptr<EventCheck> &right) {return left->e_getZSort() > right->e_getZSort(); }); }
	//void addEventCheck(std::function<bool(Vec2D)> doesEvent, std::function<void(void)> eventCallback, std::function<bool(Vec2D)>doesMouseOn, std::function<unsigned int(void)> getZSort = []() { return 0; }, std::function<bool(void)> getValidation = []() { return true; }) { this->click_checks.push_back(std::make_shared<EventCheck>(getValidation, getZSort, doesMouseOn, doesEvent, eventCallback)); std::sort(this->click_checks.begin(), this->click_checks.end(), [](const std::shared_ptr<EventCheck> &left, const std::shared_ptr<EventCheck> &right) {return left->e_getZSort() > right->e_getZSort(); });}
	void popBackObject() { this->objects.pop_back(); }
	double getShieldRatioOfUnderLayer() { return this->under_layer_shield_ratio; }
	bool doesAllowedUpdateUnderLayer() { return this->allow_update_under_layer; }
	bool doesAllowedClickCheckUnderLayer() { return this->allow_click_check_under_layer; }
	void setInvalid() { this->validation = false; }

	void initialize() override { this->validation = true; this->objects.clear(); }
	void update() override;
	virtual void draw() const { for each(std::shared_ptr<ObjectManagementBaseKit> obj in this->objects) { obj->draw(this->camera); } }

	void checkEvent(Vec2D mouse_window_pos);
};

class Layer_NowLoading : public LayerBase {
private:
	unsigned int count;
	const unsigned int min_time;
	std::function<void(void)> load_func;
	std::future<void> f;

public:
	Layer_NowLoading(std::function<void(void)> load_func, const std::shared_ptr<Camera> &camera, unsigned int min_time = 0) : min_time(min_time), LayerBase(0.0, false, false, camera) { this->load_func = load_func; this->initialize(); }
	void initialize() override { this->count = 0; this->f = std::async(this->load_func); }
	void update() override { this->count++; if ((this->f.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) && (this->count > this->min_time)) this->validation = false; }
	void draw() const override {}
};

class Layer_Pause : public LayerBase {
private:
	std::shared_ptr<DisappearButton_Fix> back, title;

public:
	Layer_Pause(const Size window_size);
	void initialize() override;
	void update() override;
};

class Layer_GameOver : public LayerBase {
private:

public:
	Layer_GameOver(const Size window_size);
	void initialize() override;
	void update() override;
};


class SceneCommonData {
public:
	const Size window_size;
	int stage_num;
	std::shared_ptr<MousePointer> mouse_pointer;
	Vec2D mouse_pointer_world_pos;

	SceneCommonData(Size window_size) : window_size(window_size) { this->mouse_pointer = std::make_shared<MousePointer>(); this->stage_num = 0; }
};

class SceneBase : public RequiredFunc {
protected:
	std::shared_ptr<SceneCommonData> commonData;
	std::vector<std::shared_ptr<LayerBase>> layers;

public:
	SceneBase(const std::shared_ptr<SceneCommonData> &common) { this->commonData = common; this->initialize(); }
	virtual ~SceneBase() {}
	void pushLayer(const std::shared_ptr<LayerBase> &layer) { this->layers.push_back(layer); }
	void checkEvent(Vec2D mouse_window_pos);

	void initialize() override { this->layers.clear(); }
	void update() override;
};
