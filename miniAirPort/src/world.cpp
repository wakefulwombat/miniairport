#include "world.h"
#include "input.h"
#include <fstream>
#include <sstream>
#include <string>
#include "targetMarker.h"

void World::loadStageInfo(int stage) {
	std::string filename = "asset\\mapchip\\stage" + std::to_string(stage) + "\\stage_info.txt";
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
	std::getline(stream, col, ':');
	this->now_time.hour = std::stoi(col);
	std::getline(stream, col, ':');
	this->now_time.minute = std::stoi(col);

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
		this->target_marker_factory->addTargetMarkerInfo(Vec2D(x, y), kind);
	}
}

void World::loadStageChip(int stage) {
	std::string filename_chip = "asset\\mapchip\\stage" + std::to_string(stage) + "\\chip.csv";
	std::ifstream ifs_chip(filename_chip);
	if (!ifs_chip) return;

	std::string filename_pass = "asset\\mapchip\\stage" + std::to_string(stage) + "\\passable.csv";
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
		this->target_marker_factory->makeTargetByKind(TargetMarkerKind::Boarding);
	}
	if (Input_T::getEventInterface_mouse()->isUpOnce("middle")) {
		this->camera->setAnchorWorldPosition((this->getWorldSizePixel() / 2).toVec());
		this->camera->setAnchorWindowPosition((this->camera->getWindowPixelSize() / 2).toVec());
	}

	LayerBase::update();
}