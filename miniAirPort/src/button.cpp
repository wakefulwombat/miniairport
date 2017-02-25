#include "button.h"
#include "input.h"

Button::Button(Vec2D center, Size size, std::string text, int font_size, std::function<void(void)> callback_clicked, std::function<Vec2D(void)> get_mouse_pointer_world_pos, Color_RGB text_color, Color_RGB background_color, Color_RGB mouseover_color) : ObjectBase(center, 100, std::make_shared<InternalObjectController_None>()), callback_clicked(callback_clicked), get_mouse_pointer_world_pos(get_mouse_pointer_world_pos), text(text), text_color(text_color), background_color(background_color), mouseover_color(mouseover_color), font_size(font_size){
	this->world_pos = center;

	this->isClicked = false;
	this->isHovered = false;
}

void Button::update() {
	if (!this->validation) return;

	if (this->isClicked) {
		this->count++;
		if (this->count >= 10) {
			this->count = 0;
			this->callback_clicked();
			this->isClicked = false;
		}
	}

	this->isHovered = false;

	Vec2D mouse = this->get_mouse_pointer_world_pos();
	if (mouse.x < this->world_pos.x - this->size.width / 2) return;
	if (mouse.y < this->world_pos.y - this->size.height / 2) return;
	if (mouse.x > this->world_pos.x + this->size.width / 2) return;
	if (mouse.y > this->world_pos.y + this->size.height / 2) return;

	this->isHovered = true;

	if (Input::getKeyCodeDownOnce(KeyType::Game_Swing_OK)) {
		this->isClicked = true;
	}
}

void Button::draw(const std::shared_ptr<CameraDrawInterface> &camera) const {
	Color_RGB col;
	if (this->isHovered) {
		col = this->mouseover_color;
	}
	else {
		col = this->background_color;
	}
	camera->drawRotateSquareInWorld(this->world_pos, this->size, 0.0, col);
	camera->drawStringInWorld(this->world_pos, this->text, this->text_color);
}



RadioButton::RadioButton(Vec2D center, Size size, std::string text, int font_size, std::function<void(void)> callback_afterOn, std::function<Vec2D(void)> get_mouse_pointer_world_pos, Color_RGB text_color, Color_RGB background_color_off, Color_RGB background_color_on, Color_RGB mouseover_color) : ObjectBase(center, 100, std::make_shared<InternalObjectController_None>()), callback_afterOn(callback_afterOn), get_mouse_pointer_world_pos(get_mouse_pointer_world_pos), text(text), text_color(text_color), background_color_on(background_color_on), background_color_off(background_color_off), mouseover_color(mouseover_color), font_size(font_size) {
	this->is_on = false;
}

void RadioButton::update() {
	if (!this->validation) return;

	Vec2D mouse = this->get_mouse_pointer_world_pos();
	if (mouse.x < this->world_pos.x - this->size.width / 2) return;
	if (mouse.y < this->world_pos.y - this->size.height / 2) return;
	if (mouse.x > this->world_pos.x + this->size.width / 2) return;
	if (mouse.y > this->world_pos.y + this->size.height / 2) return;

	if (Input::getKeyCodeDownOnce(KeyType::Game_Swing_OK)) {
		this->is_on = !this->is_on;
	}
}

void RadioButton::draw(const std::shared_ptr<CameraDrawInterface> &camera) const {
	Color_RGB col;

	if (this->is_on) {
		col = this->background_color_on;
	}
	else {
		col = this->background_color_off;
	}
	camera->drawRotateSquareInWorld(this->world_pos, this->size, 0.0, col);
	camera->drawStringInWorld(this->world_pos, this->text, this->text_color);
}