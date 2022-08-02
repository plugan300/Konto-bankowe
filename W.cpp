#include "W.h"

W::W(int id_color, std::string text) {
	this->id_color = id_color;
	this->text = text;
}


int* W::get_id_color() {
	return &id_color;
}
std::string* W::get_text() {
	return &text;
}