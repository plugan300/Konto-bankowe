#include <string>

#pragma once
class W {
public:
	W(int id_color, std::string text);

	int* get_id_color();
	std::string* get_text();
private:
	int id_color;
	std::string text;
};

