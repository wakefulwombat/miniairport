#include "table.h"

Column::Column(std::function<void(const std::shared_ptr<ObjectBase>&)> addObject, int header_height, Size cell_size,
			   Size img_size, int graph_handle, Color_RGB back_color, double img_exp, bool fill)
					:addObject(addObject), cell_size(cell_size) {
	this->initialize();
	this->header = std::make_shared<ImageCell>(Vec2D(), Size(cell_size.width, header_height), img_size, graph_handle, back_color, img_exp, fill);
	this->addObject(this->header);
}

Column::Column(std::function<void(const std::shared_ptr<ObjectBase>&)> addObject, int header_height, Size cell_size,
			   std::string text, Color_RGB back_color, Color_RGB text_color, bool fill)
					: addObject(addObject), cell_size(cell_size) {
	this->initialize();
	this->header = std::make_shared<TextCell>(Vec2D(), Size(cell_size.width, header_height), text, back_color, text_color, fill);
	this->addObject(this->header);
}

int Table::makeColumnTextHeader(int header_height, Size cell_size, std::string text, Color_RGB back_color, Color_RGB text_color, bool fill) {
	this->columns.push_back(std::make_shared<Column>(this->addObject, header_height, cell_size, text, back_color, text_color, fill));
}

int Table::makeColumnImageHeader(int header_height, Size cell_size, Size img_size, int graph_handle, Color_RGB back_color, double img_exp, bool fill) {
	this->columns.push_back(std::make_shared<Column>(this->addObject, header_height, cell_size, img_size, graph_handle, back_color, img_exp, fill));
}