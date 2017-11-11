#pragma once

#include "common.h"
#include "base_objectBase.h"
#include <string>
#include <functional>
#include <memory>

class Button : public ObjectBase {
protected:
	const std::string text;
	const Color_RGB text_color;
	const Color_RGB background_color;
	const Color_RGB mouseover_color;
	const Size size;
	const int font_size;

	const std::function<void(void)> external_callback;

	bool isHovered;
	unsigned int count;

public:
	Button(Vec2D center, Size size, std::string text, int font_size, std::function<void(void)> external_callback, Color_RGB text_color = Color_RGB(255, 255, 255), Color_RGB background_color = Color_RGB(100, 100, 100), Color_RGB mouseover_color = Color_RGB(150, 150, 150));
	void initialize() override {}
	void update() override;
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override;
	void finalize() override {}

	bool doesEvent(Vec2D e_world_pos, Vec2D e_window_pos) override;
	bool doesMouseOn(Vec2D e_world_pos, Vec2D e_window_pos) override { return e_window_pos.isInSquare(this->world_pos, this->size); }
	void eventCallback() override { this->external_callback(); }
};

class IconButton_Fix : public ObjectBase, public ImageProperty {
protected:
	const Color_RGB background_color;
	const Color_RGB mouseover_color;
	const Size size;
	const int handle;

	std::function<void(void)> external_callback;

	bool isHovered;
	unsigned int count;

	void setControlRights(ControlStatus status) override final { this->control_status = status; }

public:
	IconButton_Fix(Vec2D center, Size button_size, Size img_size, int graph_handle, std::function<void(void)> external_callback, Color_RGB background_color = Color_RGB(100, 100, 100), Color_RGB mouseover_color = Color_RGB(150, 150, 150));
	void initialize() override {}
	void update() override;
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override;
	void finalize() override {}

	bool doesEvent(Vec2D e_world_pos, Vec2D e_window_pos) override;
	bool doesMouseOn(Vec2D e_world_pos, Vec2D e_window_pos) override { return e_window_pos.isInSquare(this->world_pos, this->size); }
	void eventCallback() override { this->external_callback(); }
};

class DisappearButton_Fix : public ObjectBase {
protected:
	const std::string text;
	const Color_RGB text_color;
	const Color_RGB background_color;
	const Color_RGB mouseover_color;
	const Size size;
	const int font_size;

	const std::function<void(void)> external_callback;

	bool isHovered;
	unsigned int count;

public:
	DisappearButton_Fix(Vec2D center, Size size, std::string text, int font_size, std::function<void(void)> external_callback, Color_RGB text_color = Color_RGB(255, 255, 255), Color_RGB background_color = Color_RGB(100, 100, 100), Color_RGB mouseover_color = Color_RGB(150, 150, 150));
	void initialize() override {}
	void update() override;
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override;
	void finalize() override {}

	bool doesEvent(Vec2D e_world_pos, Vec2D e_window_pos) override;
	bool doesMouseOn(Vec2D e_world_pos, Vec2D e_window_pos) override { return e_window_pos.isInSquare(this->world_pos, this->size); }
	void eventCallback() override;
};

class RadioButton : public ObjectBase {
protected:
	const std::string text;
	const Color_RGB text_color;
	const Color_RGB background_color_on;
	const Color_RGB background_color_off;
	const Color_RGB mouseover_color;
	const Size size;
	const int font_size;

	bool is_on;
	unsigned int count;

public:
	RadioButton(Vec2D center, Size size, std::string text, int font_size, Color_RGB text_color = Color_RGB(255, 255, 255), Color_RGB background_color_off = Color_RGB(100, 100, 100), Color_RGB background_color_on = Color_RGB(200, 50, 50), Color_RGB mouseover_color = Color_RGB(150, 150, 150));
	void initialize() override {}
	void update() override;
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override;
	void finalize() override {}

	bool doesEvent(Vec2D e_world_pos, Vec2D e_window_pos) override;
	bool doesMouseOn(Vec2D e_world_pos, Vec2D e_window_pos) override { return e_window_pos.isInSquare(this->world_pos, this->size); }
	void eventCallback() override;

	bool isOn() { return this->is_on; }
};

class RadioButton_Fix : public ObjectBase {
protected:
	const std::string text;
	const Color_RGB text_color;
	const Color_RGB background_color_on;
	const Color_RGB background_color_off;
	const Color_RGB mouseover_color;
	const Size size;
	const int font_size;

	bool is_on;
	unsigned int count;

public:
	RadioButton_Fix(Vec2D center, Size size, std::string text, int font_size, Color_RGB text_color = Color_RGB(255, 255, 255), Color_RGB background_color_off = Color_RGB(100, 100, 100), Color_RGB background_color_on = Color_RGB(200, 50, 50), Color_RGB mouseover_color = Color_RGB(150, 150, 150));
	void initialize() override {}
	void update() override;
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override;
	void finalize() override {}

	bool doesEvent(Vec2D e_world_pos, Vec2D e_window_pos) override;
	bool doesMouseOn(Vec2D e_world_pos, Vec2D e_window_pos) override { return e_window_pos.isInSquare(this->world_pos, this->size); }
	void eventCallback() override;

	bool isOn() { return this->is_on; }
};

class IconRadioButton_Fix : public ObjectBase, public ImageProperty {
protected:
	const Color_RGB background_color_on;
	const Color_RGB background_color_off;
	const Size size;
	const int handle;

	const std::function<void(void)> callback_afterOn;

	bool is_on;

	void setControlRights(ControlStatus status) override final { this->control_status = status; }

public:
	IconRadioButton_Fix(Vec2D center, Size button_size,Size img_size, int graph_handle, std::function<void(void)> callback_on, Color_RGB background_color_off = Color_RGB(100, 100, 100), Color_RGB background_color_on = Color_RGB(50, 200, 50));
	void initialize() override {}
	void update() override;
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override;
	void finalize() override {}

	bool doesEvent(Vec2D e_world_pos, Vec2D e_window_pos) override;
	bool doesMouseOn(Vec2D e_world_pos, Vec2D e_window_pos) override { return e_window_pos.isInSquare(this->world_pos, this->size); }
	void eventCallback() override;

	bool isOn() { return this->is_on; }
	void setOff() { this->is_on = false; }
};