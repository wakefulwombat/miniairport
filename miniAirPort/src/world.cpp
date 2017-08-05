#include "world.h"
#include "input.h"
#include <fstream>
#include <sstream>
#include <string>
#include "targetMarker.h"
#include "resource.h"

World::World(int stage, std::function<void(const std::shared_ptr<LayerBase>&)> push_layer) : LayerBase(0.0, false, false, std::make_shared<Camera>(Size(1280, 720))), push_layer(push_layer) {

	this->time_table = std::make_shared<TimeTable>([this](std::shared_ptr<ObjectBase> obj) {this->addObject(obj); }, [this]() {return this->getWorldSizePixel(); }, [this]() {return this->isHighSpeedNow(); });
	this->high_speed = std::make_shared<RadioButton_Fix>(Vec2D(1240, 680), Size(40, 40), "=", 10);
	this->show_arrivalTimeTable = std::make_shared<IconRadioButton_Fix>(Vec2D(40, 680), Size(40, 40), Size(32, 32), Resources::getImage()->getIconArrival(), [this]() {this->offIconButtonDeparture(); });
	this->show_departureTimeTable = std::make_shared<IconRadioButton_Fix>(Vec2D(100, 680), Size(40, 40), Size(32, 32), Resources::getImage()->getIconDeparture(), [this]() {this->offIconButtonArrival(); });
	this->initialize();

	this->loadStageInfo(stage);
	this->loadStageTimeTable(stage);
	this->loadStageChip(stage);

	this->camera->setWorldSize(this->getWorldSizePixel());
	this->time_table->addTimerBoxToObject();
	this->addObject(this->high_speed);
	this->addEventCheck(this->high_speed);
	this->addObject(this->show_arrivalTimeTable);
	this->addEventCheck(this->show_arrivalTimeTable);
	this->addObject(this->show_departureTimeTable);
	this->addEventCheck(this->show_departureTimeTable);
}

void World::loadStageInfo(int stage) {
	std::string filename = "asset\\map\\stage" + std::to_string(stage) + "\\stage_info.txt";
	std::ifstream ifs(filename);
	if (!ifs) return;

	std::string line, col;
	std::istringstream stream;

	std::getline(ifs, line);
	std::getline(ifs, line);
	stream = std::istringstream(line);
	std::getline(stream, col, ',');
	this->worldSize_chipNum.width = std::stoi(col);
	std::getline(stream, col, ',');
	this->worldSize_chipNum.height = std::stoi(col);

	std::getline(ifs, line);
	std::getline(ifs, line);
	stream = std::istringstream(line);
	std::getline(stream, col, ',');
	this->chipSize_pixel.width = std::stoi(col);
	std::getline(stream, col, ',');
	this->chipSize_pixel.height = std::stoi(col);

	std::getline(ifs, line);
	std::getline(ifs, line);
	stream = std::istringstream(line);
	Time24 t;
	std::getline(stream, col, ':');
	t.hour = std::stoi(col);
	std::getline(stream, col, ':');
	t.minute = std::stoi(col);
	this->time_table->setNowTime(t);

	std::getline(ifs, line);
	std::getline(ifs, line);
	int count = std::stoi(line);
	for (int i = 0; i < count; ++i) {
		std::getline(ifs, line);
		stream = std::istringstream(line);
		std::getline(stream, col, ',');
		TargetMarkerKind kind = (TargetMarkerKind)std::stoi(col);
		std::getline(stream, col, ',');
		int x = std::stoi(col);
		std::getline(stream, col, ',');
		int y = std::stoi(col);
		this->time_table->addTargetMarkerInfo(Vec2D(x, y), kind);
	}
}

void World::loadStageTimeTable(int stage) {
	std::string filename = "asset\\map\\stage" + std::to_string(stage) + "\\timetable.csv";
	std::ifstream ifs(filename);
	if (!ifs) return;

	std::string line, col;
	std::istringstream stream;
	std::getline(ifs, line);

	std::getline(ifs, line);
	while (!ifs.eof()) {
		stream = std::istringstream(line);

		Time24 time;
		std::getline(stream, col, ',');
		time.hour = std::stoi(col);
		std::getline(stream, col, ',');
		time.minute = std::stoi(col);

		PlaneCode code;
		std::getline(stream, col, ',');
		code = (PlaneCode)std::stoi(col);

		bool b;
		std::getline(stream, col, ',');
		b = (std::stoi(col) == 1);

		this->time_table->addTimeTableInfo(time, code, b);

		std::getline(ifs, line);
	}
}

void World::loadStageChip(int stage) {
	std::string filename_chip = "asset\\map\\stage" + std::to_string(stage) + "\\chip.csv";
	std::ifstream ifs_chip(filename_chip);
	if (!ifs_chip) return;

	std::string filename_pass = "asset\\map\\stage" + std::to_string(stage) + "\\passable.csv";
	std::ifstream ifs_pass(filename_pass);
	if (!ifs_pass) return;

	std::string line, col_chip, col_pass;
	std::istringstream stream_chip, stream_pass;

	for (int y = 0; y < this->worldSize_chipNum.height; ++y) {
		std::getline(ifs_chip, line);
		stream_chip = std::istringstream(line);
		std::getline(ifs_pass, line);
		stream_pass = std::istringstream(line);
		for (int x = 0; x < this->worldSize_chipNum.width; ++x) {
			std::getline(stream_chip, col_chip, ',');
			std::getline(stream_pass, col_pass, ',');

			this->objects.push_back(std::make_shared<MapChip>(
					Vec2D((0.5+x)*this->chipSize_pixel.width, (0.5+y)*this->chipSize_pixel.height),
					this->chipSize_pixel,
					std::stoi(col_chip),
					std::stoi(col_pass)==0
				));
		}
	}
}

void World::update() {
	if (this->high_speed->isOn()) this->time_table->changeIncrementMilliSecond(500);
	else this->time_table->changeIncrementMilliSecond(100);
	this->time_table->update();

	if (Input_T::getEventInterface_mouse()->isDownOnce("left") || Input_T::getEventInterface_mouse()->isUpOnce("left")) {
		this->camera->setAnchorWorldPosition(this->camera->toWorldPosFromWindowPosPx(Input_T::getOperationInterface_mouse()->getPointerPosition()));
	}
	if (Input_T::getEventInterface_mouse()->isDown("left")) {
		this->camera->setAnchorWindowPosition(Input_T::getOperationInterface_mouse()->getPointerPosition());
	}
	
	int wheel = Input_T::getOperationInterface_mouse()->getWheelRotationDelta();
	if (wheel != 0) {
		this->camera->setAnchorWorldPosition(this->camera->toWorldPosFromWindowPosPx(Input_T::getOperationInterface_mouse()->getPointerPosition()));
		this->camera->setAnchorWindowPosition(Input_T::getOperationInterface_mouse()->getPointerPosition());
		this->camera->setZoom((1.0 + 0.3*wheel)*this->camera->getZoom(), 10);
	}
	
	if (Input_T::getEventInterface_mouse()->isKeepDownOnce("middle", 60)) {
		//ƒ|[ƒY‰æ–Ê
		this->push_layer(std::make_shared<Layer_Pause>(this->getCamera()->getWindowPixelSize()));
	}
	if (Input_T::getEventInterface_mouse()->isUpOnce("middle")) {
		this->camera->setAnchorWorldPosition((this->getWorldSizePixel() / 2).toVec());
		this->camera->setAnchorWindowPosition((this->camera->getWindowPixelSize() / 2).toVec());
	}

	LayerBase::update();
}