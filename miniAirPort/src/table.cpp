#include "table.h"

Column::Column(int header_height, Size cell_size,
			   Size img_size, int graph_handle, Color_RGB back_color, double img_exp, bool fill)
					: cell_size(cell_size) {
	this->initialize();
	this->header = std::make_shared<ImageCell>(Vec2D(), Size(cell_size.width, header_height), img_size, graph_handle, back_color, img_exp, fill);
}

Column::Column(int header_height, Size cell_size,
			   std::string text, Color_RGB back_color, Color_RGB text_color, bool fill)
					: cell_size(cell_size) {
	this->initialize();
	this->header = std::make_shared<TextCell>(Vec2D(), Size(cell_size.width, header_height), text, back_color, text_color, fill);
}

int Table::makeColumnTextHeader(int cell_width, std::string text, Color_RGB back_color, Color_RGB text_color, bool fill) {
	this->columns.push_back(std::make_shared<Column>(this->header_height, Size(cell_width, this->cell_height), text, back_color, text_color, fill));
	return this->columns.size() - 1;
}

int Table::makeColumnImageHeader(int cell_width, Size img_size, int graph_handle, Color_RGB back_color, double img_exp, bool fill) {
	this->columns.push_back(std::make_shared<Column>(this->header_height, Size(cell_width, this->cell_height), img_size, graph_handle, back_color, img_exp, fill));
	return this->columns.size() - 1;
}

int Table::addTextCell(unsigned int column_index, std::string text, Color_RGB back_color, Color_RGB text_color, bool fill) {
	if ((column_index < 0) || (column_index >= this->columns.size())) return -1;

	this->columns[column_index]->pushBackTextCell(text, back_color, text_color, fill);
	if (this->columns[column_index]->getRowNumberSize() < this->show_row_max) this->columns[column_index]->addObjectToList(this->columns[column_index]->getRowNumberSize() - 1, this->addObject);
	return this->columns[column_index]->getRowNumberSize();
}

int Table::addImageCell(unsigned int column_index, Size img_size, int graph_handle, Color_RGB back_color, double img_exp, bool fill) {
	if ((column_index < 0) || (column_index >= this->columns.size())) return -1;

	this->columns[column_index]->pushBackImageCell(img_size, graph_handle, back_color, img_exp, fill);
	if (this->columns[column_index]->getRowNumberSize() < this->show_row_max) this->columns[column_index]->addObjectToList(this->columns[column_index]->getRowNumberSize() - 1, this->addObject);
	return this->columns[column_index]->getRowNumberSize();
}

void Table::popFrontRow() {
	for (unsigned int i = 0; i < this->columns.size(); ++i) {
		if (this->columns[i]->getRowNumberSize() == 0) continue;
		this->columns[i]->popFrontCell();

		if (this->columns[i]->getRowNumberSize() >= this->show_row_max) this->columns[i]->addObjectToList(this->show_row_max, this->addObject);
	}
}

void Table::formatTable(Vec2D leftup_world_pos) {
	leftup_world_pos.y += this->header_height / 2;
	for (auto it = this->columns.begin(); it != this->columns.end(); ++it) {
		leftup_world_pos.x += (*it)->getWidthPixelSize();
		(*it)->changePosition(leftup_world_pos);
		leftup_world_pos.x += (*it)->getWidthPixelSize();
	}
}
