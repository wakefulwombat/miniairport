#include "button.h"
#include "input.h"

Button::Button(Vec2D center, Size size, std::string text, int font_size, std::function<void(void)> external_callback, std::function<Vec2D(void)> get_mouse_pointer_world_pos, Color_RGB text_color, Color_RGB background_color, Color_RGB mouseover_color) : ObjectBase(center, 1000), size(size), external_callback(external_callback), get_mouse_pointer_world_pos(get_mouse_pointer_world_pos), text(text), text_color(text_color), background_color(background_color), mouseover_color(mouseover_color), font_size(font_size){
	this->world_pos = center;
	this->isHovered = false;
}

void Button::update() {
	if (!this->validation) return;
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

bool Button::doesEvent(Vec2D e_window_pos)
{
	this->isHovered = false;

	Vec2D mouse = this->get_mouse_pointer_world_pos();
	if (mouse.x < this->world_pos.x - this->size.width / 2) return false;
	if (mouse.y < this->world_pos.y - this->size.height / 2) return false;
	if (mouse.x > this->world_pos.x + this->size.width / 2) return false;
	if (mouse.y > this->world_pos.y + this->size.height / 2) return false;

	this->isHovered = true;

	if (Input_T::getEventInterface_mouse()->isDownOnce("left")) {
		return true;
	}
	return false;
}

void Button::eventCallback()
{
	this->external_callback();
}



IconButton_Fix::IconButton_Fix(Vec2D center, Size button_size, Size img_size,int graph_handle, std::function<void(void)> external_callback, Color_RGB background_color, Color_RGB mouseover_color) : ObjectBase(center, 1000), ImageProperty(std::make_shared<ImagePropertyData>(img_size)), size(button_size), handle(graph_handle), external_callback(external_callback), background_color(background_color), mouseover_color(mouseover_color) {
	this->world_pos = center;

	this->isHovered = false;
}

void IconButton_Fix::update() {
	if (!this->validation) return;
}

void IconButton_Fix::draw(const std::shared_ptr<CameraDrawInterface> &camera) const {
	Color_RGB col;
	if (this->isHovered) {
		col = this->mouseover_color;
	}
	else {
		col = this->background_color;
	}
	camera->drawRotateSquareOnWindowFixed(this->world_pos, this->size, 0.0, col);
	camera->drawImageOnWindow(this->world_pos, this->handle, this->img_prop);
}

bool IconButton_Fix::doesEvent(Vec2D e_window_pos)
{
	this->isHovered = false;

	if (e_window_pos.x < this->world_pos.x - this->size.width / 2) return false;
	if (e_window_pos.y < this->world_pos.y - this->size.height / 2) return false;
	if (e_window_pos.x > this->world_pos.x + this->size.width / 2) return false;
	if (e_window_pos.y > this->world_pos.y + this->size.height / 2) return false;

	this->isHovered = true;

	if (Input_T::getEventInterface_mouse()->isDownOnce("left")) {
		return true;
	}
	return false;
}

void IconButton_Fix::eventCallback()
{
	this->external_callback();
}



DisappearButton_Fix::DisappearButton_Fix(Vec2D center, Size size, std::string text, int font_size, std::function<void(void)> external_callback, Color_RGB text_color, Color_RGB background_color, Color_RGB mouseover_color) : ObjectBase(center, 1000), size(size), external_callback(external_callback), text(text), text_color(text_color), background_color(background_color), mouseover_color(mouseover_color), font_size(font_size) {
	this->world_pos = center;

	this->isHovered = false;
}

void DisappearButton_Fix::update() {
	if (!this->validation) return;
}

void DisappearButton_Fix::draw(const std::shared_ptr<CameraDrawInterface> &camera) const {
	Color_RGB col;
	if (this->isHovered) {
		col = this->mouseover_color;
	}
	else {
		col = this->background_color;
	}
	camera->drawRotateSquareOnWindowFixed(this->world_pos, this->size, 0.0, col);
	camera->drawStringOnWindowFixed(this->world_pos, this->text, this->text_color);
}

bool DisappearButton_Fix::doesEvent(Vec2D e_window_pos)
{
	this->isHovered = false;

	if (e_window_pos.x < this->world_pos.x - this->size.width / 2) return false;
	if (e_window_pos.y < this->world_pos.y - this->size.height / 2) return false;
	if (e_window_pos.x > this->world_pos.x + this->size.width / 2) return false;
	if (e_window_pos.y > this->world_pos.y + this->size.height / 2) return false;

	this->isHovered = true;

	if (Input_T::getEventInterface_mouse()->isDownOnce("left")) {
		return true;
	}
	return false;
}

void DisappearButton_Fix::eventCallback()
{
	this->setInvalid();
	this->external_callback();
}



RadioButton::RadioButton(Vec2D center, Size size, std::string text, int font_size, std::function<Vec2D(void)> get_mouse_pointer_world_pos, Color_RGB text_color, Color_RGB background_color_off, Color_RGB background_color_on, Color_RGB mouseover_color) : ObjectBase(center, 1000), size(size), get_mouse_pointer_world_pos(get_mouse_pointer_world_pos), text(text), text_color(text_color), background_color_on(background_color_on), background_color_off(background_color_off), mouseover_color(mouseover_color), font_size(font_size) {
	this->is_on = false;
}

void RadioButton::update() {
	if (!this->validation) return;
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

bool RadioButton::doesEvent(Vec2D e_window_pos)
{
	Vec2D mouse = this->get_mouse_pointer_world_pos();
	if (mouse.x < this->world_pos.x - this->size.width / 2) return false;
	if (mouse.y < this->world_pos.y - this->size.height / 2) return false;
	if (mouse.x > this->world_pos.x + this->size.width / 2) return false;
	if (mouse.y > this->world_pos.y + this->size.height / 2) return false;

	if (Input_T::getEventInterface_mouse()->isDownOnce("left")) {
		return true;
	}
	return false;
}

void RadioButton::eventCallback()
{
	this->is_on = !this->is_on;
}


RadioButton_Fix::RadioButton_Fix(Vec2D center, Size size, std::string text, int font_size, Color_RGB text_color, Color_RGB background_color_off, Color_RGB background_color_on, Color_RGB mouseover_color) : ObjectBase(center, 1000), size(size), text(text), text_color(text_color), background_color_on(background_color_on), background_color_off(background_color_off), mouseover_color(mouseover_color), font_size(font_size) {
	this->is_on = false;
}

void RadioButton_Fix::update() {
	if (!this->validation) return;
}

void RadioButton_Fix::draw(const std::shared_ptr<CameraDrawInterface> &camera) const {
	Color_RGB col;

	if (this->is_on) {
		col = this->background_color_on;
	}
	else {
		col = this->background_color_off;
	}
	camera->drawRotateSquareOnWindowFixed(this->world_pos, this->size, 0.0, col);
	camera->drawStringOnWindowFixed(this->world_pos, this->text, this->text_color);
}

bool RadioButton_Fix::doesEvent(Vec2D e_window_pos)
{
	if (e_window_pos.x < this->world_pos.x - this->size.width / 2) return false;
	if (e_window_pos.y < this->world_pos.y - this->size.height / 2) return false;
	if (e_window_pos.x > this->world_pos.x + this->size.width / 2) return false;
	if (e_window_pos.y > this->world_pos.y + this->size.height / 2) return false;

	if (Input_T::getEventInterface_mouse()->isDownOnce("left")) {
		return true;
	}
	return false;
}

void RadioButton_Fix::eventCallback()
{
	this->is_on = !this->is_on;
}



IconRadioButton_Fix::IconRadioButton_Fix(Vec2D center, Size button_size, Size img_size, int graph_handle, std::function<void(void)> callback_afterOn, Color_RGB background_color_off, Color_RGB background_color_on) : ObjectBase(center, 1000), ImageProperty(std::make_shared<ImagePropertyData>(img_size)), size(button_size), handle(graph_handle), callback_afterOn(callback_afterOn), background_color_on(background_color_on), background_color_off(background_color_off) {
	this->is_on = false;
}

void IconRadioButton_Fix::update() {
	if (!this->validation) return;
}

void IconRadioButton_Fix::draw(const std::shared_ptr<CameraDrawInterface> &camera) const {
	Color_RGB col;

	if (this->is_on) {
		col = this->background_color_on;
	}
	else {
		col = this->background_color_off;
	}
	camera->drawRotateSquareOnWindowFixed(this->world_pos, this->size, 0.0, col);
	camera->drawImageOnWindow(this->world_pos, this->handle, this->img_prop);
}

bool IconRadioButton_Fix::doesEvent(Vec2D e_window_pos)
{
	if (e_window_pos.x < this->world_pos.x - this->size.width / 2) return false;
	if (e_window_pos.y < this->world_pos.y - this->size.height / 2) return false;
	if (e_window_pos.x > this->world_pos.x + this->size.width / 2) return false;
	if (e_window_pos.y > this->world_pos.y + this->size.height / 2) return false;

	if (Input_T::getEventInterface_mouse()->isDownOnce("left")) {
		return true;
	}
	return false;
}

void IconRadioButton_Fix::eventCallback()
{
	this->is_on = !this->is_on;
	if (this->is_on) {
		this->callback_afterOn();
	}
}
