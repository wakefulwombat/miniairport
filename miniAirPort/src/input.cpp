#include "input.h"
#include "common.h"
#include "DxLib.h"
#include <stdio.h>
#include <math.h>

KeyCodeState Input::keyCodeDownCount;
KeyCodeState Input::keyCodeUpCount;
int Input::mouse_wheel_rotation = 0;
Vec2D Input::now_mouse = Vec2D(-1, -1);


void Input::initialize() {
	for (int i = 0; i < 4; ++i) {
		Input::keyCodeDownCount.key[i] = 1;
		Input::keyCodeUpCount.key[i] = 1;
	}
}

void Input::update() {
	//キーボード
	char key[256];
	GetHitKeyStateAll(key);

	/*if (key[KEY_INPUT_P] != 0) {
		Input::keyCodeDownCount.key[(int)KeyType::Common_Pause]++;
		Input::keyCodeUpCount.key[(int)KeyType::Common_Pause] = 0;
	}
	else {
		Input::keyCodeUpCount.key[(int)KeyType::Common_Pause]++;
		Input::keyCodeDownCount.key[(int)KeyType::Common_Pause] = 0;
	}*/

	if (key[KEY_INPUT_SPACE] != 0) {
		Input::keyCodeDownCount.key[(int)KeyType::Game_Slow]++;
		Input::keyCodeUpCount.key[(int)KeyType::Game_Slow] = 0;
	}
	else {
		Input::keyCodeUpCount.key[(int)KeyType::Game_Slow]++;
		Input::keyCodeDownCount.key[(int)KeyType::Game_Slow] = 0;
	}

	//マウス
	int x, y;
	GetMousePoint(&x, &y);
	Input::now_mouse.x = x;
	Input::now_mouse.y = y;

	int mouse = GetMouseInput();
	if (mouse&MOUSE_INPUT_LEFT) {//OK or Swing
		Input::keyCodeDownCount.key[(int)KeyType::Game_Swing_OK]++;
		Input::keyCodeUpCount.key[(int)KeyType::Game_Swing_OK] = 0;
	}
	else {
		Input::keyCodeDownCount.key[(int)KeyType::Game_Swing_OK]=0;
		Input::keyCodeUpCount.key[(int)KeyType::Game_Swing_OK]++;
	}

	if (mouse&MOUSE_INPUT_RIGHT) {//Cancel or Trans
		Input::keyCodeDownCount.key[(int)KeyType::Game_VectorTrans_CANCEL]++;
		Input::keyCodeUpCount.key[(int)KeyType::Game_VectorTrans_CANCEL] = 0;
	}
	else {
		Input::keyCodeDownCount.key[(int)KeyType::Game_VectorTrans_CANCEL] = 0;
		Input::keyCodeUpCount.key[(int)KeyType::Game_VectorTrans_CANCEL]++;
	}

	if (mouse&MOUSE_INPUT_MIDDLE) {//pause
		Input::keyCodeDownCount.key[(int)KeyType::Common_Pause]++;
		Input::keyCodeUpCount.key[(int)KeyType::Common_Pause] = 0;
	}
	else {
		Input::keyCodeDownCount.key[(int)KeyType::Common_Pause] = 0;
		Input::keyCodeUpCount.key[(int)KeyType::Common_Pause]++;
	}

	Input::mouse_wheel_rotation = GetMouseWheelRotVol();
}

bool Input::getKeyCodeDown(KeyType key){
	return (Input::keyCodeDownCount.key[(int)key] != 0);
}

bool Input::getKeyCodeDownOnce(KeyType key){
	return (Input::keyCodeDownCount.key[(int)key] == 1);
}

bool Input::getKeyCodeUpOnce(KeyType key){
	return (Input::keyCodeUpCount.key[(int)key] == 1);
}

int Input::getMouseWheelRotation() {
	return Input::mouse_wheel_rotation;
}

Vec2D Input::getNowMousePoint() {
	return Input::now_mouse;
}

bool Input::isObjectOnMouse(Vec2D leftup, Size size) {
	if (Input::now_mouse.x < leftup.x) return false;
	if (Input::now_mouse.y < leftup.y) return false;
	if (Input::now_mouse.x + size.width > leftup.x) return false;
	if (Input::now_mouse.y + size.height > leftup.y) return false;
	return true;
}

bool Input::isObjectLeftClicked(Vec2D leftup, Size size) {
	if (Input::getKeyCodeDownOnce(KeyType::Game_Swing_OK) != 0) return false;

	if (Input::now_mouse.x < leftup.x) return false;
	if (Input::now_mouse.y < leftup.y) return false;
	if (Input::now_mouse.x + size.width > leftup.x) return false;
	if (Input::now_mouse.y + size.height > leftup.y) return false;
	return true;
}

bool Input::isObjectRightClicked(Vec2D leftup, Size size) {
	if (Input::getKeyCodeDownOnce(KeyType::Game_VectorTrans_CANCEL) != 0) return false;

	if (Input::now_mouse.x < leftup.x) return false;
	if (Input::now_mouse.y < leftup.y) return false;
	if (Input::now_mouse.x + size.width > leftup.x) return false;
	if (Input::now_mouse.y + size.height > leftup.y) return false;
	return true;
}

void Input::setNowMousePoint(Vec2D pos) {
	Input::now_mouse = pos;
	SetMousePoint((int)pos.x, (int)pos.y);
}