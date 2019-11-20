#include "Parser.hpp"

void Txt_Parser::set_width() {
  string line;
  getline(fin, line);
  width = line.size() - 2;
}

void Txt_Parser::set_height() {
  string line;
  getline(fin, line);
  while (line != "]") height++;
  height -= 2;
}
Map Txt_Parser::get_map() {}
