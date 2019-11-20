#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "Agent.hpp"
#include "Map.hpp"

using namespace std;

class Map_parser {
 protected:
  ifstream fin;
  string filename;

 public:
  Map_parser(string name) : filename(name) {
    fin.open(filename);
    if (!fin.is_open()) {
      cerr << "File " << filename << " was not found";
      exit(getchar());
    }
  };
  virtual Map get_map() = 0;
  virtual list<Agent> get_agents_list();
};

class Txt_Parser : Map_parser {
 private:
  uint width, height;
  vector<pair<uint,uint>> obstacles;
  void set_width();
  void set_height();
 public:
	 Txt_Parser(string name) : Map_parser(name) {
		width = height = 0;
		obstacles.clear();
	 };
	 Map get_map() override;
	 list<Agent> get_agents_list() override;

};
