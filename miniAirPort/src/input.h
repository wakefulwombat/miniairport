#pragma once

#include "common.h"

struct KeyCodeState{
	unsigned int key[4];
};

enum class KeyType{
	Common_Pause,
	Game_Slow,
	Game_Swing_OK,	
	Game_VectorTrans_CANCEL
};

class Input{
private:
	static KeyCodeState keyCodeDownCount;//キーを押している時間
	static KeyCodeState keyCodeUpCount;//キーを押していない時間
	static int mouse_wheel_rotation;//マウスホイールの回転状態
	static Vec2D now_mouse;

public:
	static void initialize();
	static void update();

	static bool getKeyCodeDown(KeyType key);
	static bool getKeyCodeDownOnce(KeyType key);
	static bool getKeyCodeUpOnce(KeyType key);
	static int getMouseWheelRotation();

	static Vec2D getNowMousePoint();
	static void setNowMousePoint(Vec2D pos);

	static bool isObjectOnMouse(Vec2D leftup, Size size);
	static bool isObjectLeftClicked(Vec2D leftup, Size size);
	static bool isObjectRightClicked(Vec2D leftup, Size size);
};
