#pragma once

#include <deque>
#include <vector>
#include <memory>
#include <string>

enum class Direction2 {
	Vertical,
	Horizontal
};

template <typename Tcell>
class GenerativeTable {
private:
	std::deque<std::string> header[2];
	std::deque<std::shared_ptr<Tcell>> cells;
	const Direction2 priority;

public:
	GenerativeTable(Direction2 priority) : priority(priority) { this->initialize(); }
	void initialize() { this->header[0].clear(); this->header[1].clear(); this->cells.clear(); }

	void addHeader(std::string text, Direction2 dir) { this->header[(int)dir].push_back(text); }
	void fillHeader(std::string text, Direction2 dir, unsigned int max_num) { for (unsigned int i = 0; i < max_num - this->header[(int)dir].size(); ++i) { this->addHeader(text, dir); } }

	void addBand(std::vector<std::shared_ptr<Tcell>> cs) { for (auto it = cs.begin(); it != cs.end(); ++it) { this->cells.push_back(*it); } }
};