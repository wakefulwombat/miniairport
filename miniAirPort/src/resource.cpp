#include "resource.h"
#include "DxLib.h"

std::shared_ptr<Image> Resources::img;
std::shared_ptr<BGM> Resources::bgm;
std::shared_ptr<SE> Resources::se;
std::shared_ptr<Font> Resources::font;

void Resources::initialize() {
	Resources::img = std::make_shared<Image>();
	Resources::bgm = std::make_shared<BGM>();
	Resources::se = std::make_shared<SE>();
	Resources::font = std::make_shared<Font>();
}



void Image::loadGameImage() {
	this->mousePointer = LoadGraph("asset\\pointer.png");
	LoadDivGraph("asset\\mapchip\\chip\\mapchip.bmp", 12, 4, 3, 64, 64, this->map_chip);
}

void Image::loadMenuImage() {
	this->mousePointer = LoadGraph("asset\\pointer.png");
}


Font::Font() {
	this->font_size = 30;
	SetFontSize(this->font_size);
}