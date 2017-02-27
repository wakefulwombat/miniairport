#pragma once
#include "common.h"
#include "base_objectBase.h"
#include <deque>
#include <string>

class CellBase : public ObjectBase {
protected:
	const Size size;
	const Color_RGB back_color;
	const bool fill;

public:

};

class TextCell : public CellBase {
private:
	const std::string text;
	const int font_size;

public:

};

class ImageCell : public CellBase, public ImageProperty {
private:
	const int handle;

public:

};


class Column {
private:
	std::shared_ptr<CellBase> header;
	std::deque<std::shared_ptr<CellBase>> cells;

public:
	Column();
	void initialize() { this->header = nullptr; this->cells.clear(); }

	void setHeader(const std::shared_ptr<CellBase>& header) { this->header = header; }
	void pushFrontCell(const std::shared_ptr<CellBase>& cell) { this->cells.push_front(cell); }
	void pushBackCell(const std::shared_ptr<CellBase>& cell) { this->cells.push_back(cell); }
	void popFrontCell() { this->cells.pop_front(); }
	void popBackCell() { this->cells.pop_back(); }

	int getRowSize() { return this->cells.size(); }
};


class Table {
private:
	std::deque<std::shared_ptr<Column>> columns;

public:
	Table();
	void initialize() { this->columns.clear(); }

	void pushFrontColumn(const std::shared_ptr<Column>& col) { this->columns.push_front(col); }
	void pushBackColumn(const std::shared_ptr<Column>& col) { this->columns.push_back(col); }
	void popFrontColumn() { this->columns.pop_front(); }
	void popBackColumn() { this->columns.pop_back(); }

	int getColumnSize() { return this->columns.size(); }
	int getLongestRowSize() { int l = 0; for (auto it = this->columns.begin(); it != this->columns.end(); ++it) { if ((*it)->getRowSize() > l) l = (*it)->getRowSize(); } return l; }
};