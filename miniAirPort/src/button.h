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

	const std::function<void(void)> callback_clicked;
	const std::function<Vec2D(void)> get_mouse_pointer_world_pos;

	bool isClicked, isHovered;
	unsigned int count;

public:
	Button(Vec2D center, Size size, std::string text, int font_size, std::function<void(void)> callback_clicked, std::function<Vec2D(void)> get_mouse_pointer_world_pos, Color_RGB text_color = Color_RGB(255, 255, 255), Color_RGB background_color = Color_RGB(100, 100, 100), Color_RGB mouseover_color = Color_RGB(150, 150, 150));
	void initialize() override {}
	void update() override;
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override;
	void finalize() override {}
};

class IconButton_Fix : public ObjectBase, public ImageProperty {
protected:
	const Color_RGB background_color;
	const Color_RGB mouseover_color;
	const Size size;
	const int handle;

	const std::function<void(void)> callback_clicked;

	bool isClicked, isHovered;
	unsigned int count;

	void setControlRights(ControlStatus status) override final { this->control_status = status; }

public:
	IconButton_Fix(Vec2D center, Size button_size, Size img_size, int graph_handle, std::function<void(void)> callback_clicked, Color_RGB background_color = Color_RGB(100, 100, 100), Color_RGB mouseover_color = Color_RGB(150, 150, 150));
	void initialize() override {}
	void update() override;
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override;
	void finalize() override {}
};

class DisappearButton_Fix : public ObjectBase {
protected:
	const std::string text;
	const Color_RGB text_color;
	const Color_RGB background_color;
	const Color_RGB mouseover_color;
	const Size size;
	const int font_size;

	const std::function<void(void)> callback_clicked;

	bool isHovered;
	unsigned int count;

public:
	DisappearButton_Fix(Vec2D center, Size size, std::string text, int font_size, std::function<void(void)> callback_clicked, Color_RGB text_color = Color_RGB(255, 255, 255), Color_RGB background_color = Color_RGB(100, 100, 100), Color_RGB mouseover_color = Color_RGB(150, 150, 150));
	void initialize() override {}
	void update() override;
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override;
	void finalize() override {}
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
	
	const std::function<void(void)> callback_afterOn;
	const std::function<Vec2D(void)> get_mouse_pointer_world_pos;

	bool is_on;
	unsigned int count;

public:
	RadioButton(Vec2D center, Size size, std::string text, int font_size, std::function<void(void)> callback_on, std::function<Vec2D(void)> get_mouse_pointer_world_pos, Color_RGB text_color = Color_RGB(255, 255, 255), Color_RGB background_color_off = Color_RGB(100, 100, 100), Color_RGB background_color_on = Color_RGB(200, 50, 50), Color_RGB mouseover_color = Color_RGB(150, 150, 150));
	void initialize() override {}
	void update() override;
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override;
	void finalize() override {}

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

	const std::function<void(void)> callback_afterOn;

	bool is_on;
	unsigned int count;

public:
	RadioButton_Fix(Vec2D center, Size size, std::string text, int font_size, std::function<void(void)> callback_on, Color_RGB text_color = Color_RGB(255, 255, 255), Color_RGB background_color_off = Color_RGB(100, 100, 100), Color_RGB background_color_on = Color_RGB(200, 50, 50), Color_RGB mouseover_color = Color_RGB(150, 150, 150));
	void initialize() override {}
	void update() override;
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override;
	void finalize() override {}

	bool isOn() { return this->is_on; }
};