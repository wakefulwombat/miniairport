#pragma once

#include "common.h"
#include <vector>

class MouseTracer : public RequiredFunc {
private:
	std::vector<Vec2D> world_pos;
	bool is_logging;
	double invalid_r;
	unsigned int count;

public:
	MouseTracer() { this->initialize(); };
	void initialize() override { this->is_logging = false; this->world_pos.clear(); this->count = 0; };
	void update() override;
	void finalize() override {};

	void startLogging(Vec2D start_world_pos, Vec2D next_world_pos, double invalid_r) { this->initialize(); this->is_logging = true; this->world_pos.push_back(start_world_pos); this->world_pos.push_back(next_world_pos); this->invalid_r = invalid_r; }
	void endLogging() { this->is_logging = false; }

	Vec2D getNextPosition(Vec2D world_pos, Vec2D world_vel);
};