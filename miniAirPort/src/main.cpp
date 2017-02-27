#include "DxLib.h"
#include "scene_manager.h"
#include "resource.h"
#include "input.h"
#include <memory>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);
	//SetUseDXArchiveFlag(TRUE);
	//SetDXArchiveExtension("dat");

	//if (MessageBox(NULL, "フルスクリーンで起動しますか？", "起動オプション", MB_YESNO) == IDYES){
	//ChangeWindowMode(0);
	//}
	//else{
	ChangeWindowMode(1);
	//}
	

	SetGraphMode(1280, 720, 16);
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	SetWindowText("One-Operation AirPort");


	if (DxLib_Init() == -1) return -1;
	SetDrawScreen(DX_SCREEN_BACK);
	SetMouseDispFlag(false);

	Resources::initialize();
	Input_T::initialize();
	std::shared_ptr<SceneManager> mgr = std::make_shared<SceneManager>(Size(1280, 720));

	while (1) {
		ClearDrawScreen();

		Input_T::update();
		mgr->update();

		ScreenFlip();

		if (ProcessMessage() == -1) break;
	}

	mgr->finalize();
	DxLib_End();

	return 0;
}