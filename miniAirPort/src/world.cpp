#include "world.h"
#include <fstream>
#include <sstream>
#include <string>

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