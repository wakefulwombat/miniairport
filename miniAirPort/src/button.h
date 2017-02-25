#pragma once

#include "common.h"
#include "base_objectBase.h"
#include <string>
#include <functional>
#include <memory>

class Button : public ObjectBase, public std::enable_shared_from_this<Button> {
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

class RadioButton : public ObjectBase, public std::enable_shared_from_this<RadioButton> {
private:
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
};