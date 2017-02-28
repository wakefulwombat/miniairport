#pragma once
#include "common.h"
#include "base_objectBase.h"
#include <deque>
#include <string>
#include <functional>

class CellBase : public ObjectBase {
protected:
	const Size size;
	const Color_RGB back_color;
	const bool fill;

public:
	CellBase(Vec2D center, Size cell_size, Color_RGB back_color = Color_RGB(200, 200, 200), bool fill = false) :ObjectBase(center, 10000), size(cell_size), back_color(back_color), fill(fill) {}
	void initialize() override {}
	void update() override {}
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override { camera->drawRotateSquareInWorld(this->world_pos, this->size, 0.0, this->back_color, this->fill); }

	Size getSize() { return this->size; }
};

class TextCell : public CellBase {
private:
	const std::string text;
	const Color_RGB text_color;

public:
	TextCell(Vec2D center, Size cell_size, std::string text, Color_RGB back_color = Color_RGB(200, 200, 200), Color_RGB text_color = Color_RGB(255, 255, 255), bool fill = false) :CellBase(center, cell_size, back_color, fill), text(text), text_color(text_color) {}
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override { CellBase::draw(camera); camera->drawStringInWorld(this->world_pos, this->text, this->text_color); }
};

class ImageCell : public CellBase, public ImageProperty {
private:
	const int handle;

	void setControlRights(ControlStatus status) override final { this->control_status = status; }

public:
	ImageCell(Vec2D center, Size cell_size, Size img_size, int graph_handle, Color_RGB back_color = Color_RGB(200, 200, 200), double img_exp = 1.0, bool fill = false) :CellBase(center, cell_size, back_color, fill), ImageProperty(std::make_shared<ImagePropertyData>(img_size, img_exp)), handle(graph_handle) {}
	void draw(const std::shared_ptr<CameraDrawInterface> &camera) const override { CellBase::draw(camera); camera->drawImageInWorld(this->world_pos, this->handle, this->img_prop); }
};


class Column {
private:
	const Size cell_size;
	std::shared_ptr<CellBase> header;
	std::deque<std::shared_ptr<CellBase>> cells;
	const std::function<void(const std::shared_ptr<ObjectBase>&)> addObject;

public:
	Column(std::function<void(const std::shared_ptr<ObjectBase>&)> addObject, int header_height, Size cell_size, Size img_size, int graph_handle, Color_RGB back_color = Color_RGB(200, 200, 200), double img_exp = 1.0, bool fill = false);
	Column(std::function<void(const std::shared_ptr<ObjectBase>&)> addObject, int header_height, Size cell_size, std::string text, Color_RGB back_color = Color_RGB(200, 200, 200), Color_RGB text_color = Color_RGB(255, 255, 255), bool fill = false);
	void initialize() { this->cells.clear(); }

	void pushBackImageCell(Size img_size, int graph_handle, Color_RGB back_color = Color_RGB(200, 200, 200), double img_exp = 1.0, bool fill = false) { std::shared_ptr<ImageCell> cell = std::make_shared<ImageCell>(Vec2D(), this->cell_size, img_size, graph_handle, back_color, img_exp, fill); this->cells.push_back(cell); this->addObject(cell); }
	void pushBackTextCell(std::string text, Color_RGB back_color = Color_RGB(200, 200, 200), Color_RGB text_color = Color_RGB(255, 255, 255), bool fill = false) { std::shared_ptr<TextCell> cell = std::make_shared<TextCell>(Vec2D(), this->cell_size, text, back_color, text_color, fill); this->cells.push_back(cell); this->addObject(cell); }
	void popFrontCell() { if (this->cells.size() == 0)return; this->cells[0]->setInvalid(); this->cells.pop_front(); }
	void removeAll() { for (auto it = this->cells.begin(); it != this->cells.end(); ++it) { (*it)->setInvalid(); } this->cells.clear(); this->header->setInvalid(); this->header = nullptr; }

	void changePosition(Vec2D head_center) { this->header->setWorldPosition(head_center); head_center.y += (this->header->getSize().height + this->cell_size.height) / 2; for (auto it = this->cells.begin(); it != this->cells.end(); ++it) { (*it)->setWorldPosition(head_center); head_center.y += this->cell_size.height; } }

	int getRowNumberSize() { return this->cells.size(); }
	int getWidthPixelSize() { return this->header->getSize().width; }
	int getHeightPixelSize() { return (this->header->getSize().height + this->cell_size.height*this->cells.size()); }
};


class Table {
private:
	std::deque<std::shared_ptr<Column>> columns;
	const std::function<void(const std::shared_ptr<ObjectBase>&)> addObject;

	int getLongestColumnIndex() { int l = 0; for (int i = 0; i < this->columns.size(); ++i) { if (this->columns[i]->getRowNumberSize() > this->columns[l]->getRowNumberSize()) { l = i; } } return l; }

public:
	Table(std::function<void(const std::shared_ptr<ObjectBase>&)> addObject) :addObject(addObject) {}
	void initialize() { this->columns.clear(); }

	int makeColumnTextHeader(int header_height, Size cell_size, std::string text, Color_RGB back_color = Color_RGB(200, 200, 200), Color_RGB text_color = Color_RGB(255, 255, 255), bool fill = false);
	int makeColumnImageHeader(int header_height, Size cell_size, Size img_size, int graph_handle, Color_RGB back_color = Color_RGB(200, 200, 200), double img_exp = 1.0, bool fill = false);

	int addTextCell(int column_index);
	int addImageCell(int column_index);

	int getColumnNumberSize() { return this->columns.size(); }
	int getLongestRowNumberSize() { int l = 0; for (auto it = this->columns.begin(); it != this->columns.end(); ++it) { if ((*it)->getRowNumberSize() > l) l = (*it)->getRowNumberSize(); } return l; }

	int getTableWidthPixel() { int sum = 0; for (auto it = this->columns.begin(); it != this->columns.end(); ++it) { sum += (*it)->getWidthPixelSize(); } return sum; }
	int getTableHeightPixel() { int i = this->getLongestColumnIndex(); return this->columns[i]->getHeightPixelSize(); }
};