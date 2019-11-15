#include "Map.hpp"

Map::Map(uint w, uint h) {
  width = w + 2;
  height = h + 2;
  square = width * height;
  allocate_map();
}

Map::Map(uint w, uint h, uint obstacle_percentage) {
  width = w + 2;
  height = h + 2;
  square = width * height;
  obstacles_percent = obstacle_percentage;
  allocate_map(obstacles_percent);
}

Map::Map(const Map& m) {
  height = m.get_height();
  width = m.get_width();
  square = height * width;
  char** map_data_temp = m.get_map_data();

  if (map_data = new char*[height]) {
    for (uint i = 0; i < height; i++)
      if (map_data[i] = new char[width])
        for (uint j = 0; j < width; j++) map_data[i][j] = map_data_temp[i][j];
  } else
    cerr << "Compieng constructor error\n";
}

void Map::allocate_map() {
  if (map_data = new char*[height])
    for (int i = 0; i < height; i++) {
      if (map_data[i] = new char[width])
        for (int j = 0; j < width; j++) {
          if (i > 0 && i < height - 1 && j > 0 && j < width - 1)
            map_data[i][j] = 'O';
          else
            map_data[i][j] = '#';
        }
      else
        cerr << "Map allocateing error\n";
    }
  else
    cerr << "Map allocateing error (map_data malloc)\n";
}

void Map::allocate_map(uint obstacle_percentage) {
  if (map_data = new char*[height])
    for (uint i = 0; i < height; i++)
      if (map_data[i] = new char[width])
        for (uint j = 0; j < width; j++) {
          if (i > 0 && i < height - 1 && j > 0 && j < width - 1) {
            if ((rand() % 100) < obstacle_percentage) {
              map_data[i][j] = '#';
              obstacles[{i, j}] = '#';
            } else
              map_data[i][j] = 'O';
          } else
            map_data[i][j] = '#';
        }
      else
        cerr << "Map allocating error\n";
  else
    cerr << "Map allocating error (map_data malloc)\n";
}

uint Map::get_height() const { return (height); }
uint Map::get_width() const { return (width); }
uint Map::get_square() const { return (square); }
char** Map::get_map_data() const { return (map_data); }
void Map::get_obstacles(unordered_map<Node, char>& m) { m = obstacles; }

void Map::set_obstacle(uint x, uint y) {
  if (x < width && y < height) {
    map_data[y][x] = '#';
    obstacles[{y, x}] = '#';
  }
}

void Map::set_obstacle(const vector<pair<uint, uint>>& p) {
  for (auto const& i : p) {
    if (i.first < width && i.second < height) {
      map_data[i.second][i.first] = '#';
      obstacles[{i.first, i.second}] = '#';
    } else
      cout << "node [" << i.first << ", " << i.second << "] cannot be set!\n";
  }
}

bool Map::is_obstacle(const Node& n) {
  return ((map_data[n.y][n.x] == '#') ? true : false);
}

void Map::print_map() {
  for (uint y = 0; y < height; y++) {
    for (uint x = 0; x < width; x++) {
      Node node = {x, y};
      if (is_obstacle(node)) {
        cout << "#\t";
        continue;
      }
      cout << '\t';
    }
    cout << endl;
  }
}

void Map::print_map_with_agents(const unordered_map<Node, Agent*>& m) {
  for (uint y = 0; y < height; ++y) {
    for (uint x = 0; x < width; ++x) {
      Node node = {x, y};
      if (is_obstacle(node)) {
        cout << "#\t";
        continue;
      }
      auto search = m.find(node);
      if (search != m.end()) {
        cout << search->second->get_name() << '\t';
        continue;
      }
      cout << '\t';
    }
    cout << endl;
  }
  cout << endl;
}

void Map::print_agent_path(const Agent& agent, const vector<Node>& nodes) {
  for (Node node : nodes) map_data[node.y][node.x] = agent.get_name();

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) cout << map_data[i][j];
    cout << endl;
  }
}

void Map::get_neighbors(const Node& node, vector<Node>& neighbors) {
  uint x = node.x;
  uint y = node.y;
  neighbors = {
      {x, y - 1, numeric_limits<uint>::max()},
      {x - 1, y, numeric_limits<uint>::max()},
      {x + 1, y, numeric_limits<uint>::max()},
      {x, y + 1, numeric_limits<uint>::max()},
  };
}

void Map::get_clean_neighbors(const Node& node, vector<Node>& nodes) {
  uint x = node.x;
  uint y = node.y;
  Node node_neighbor;
  node_neighbor = {x, y - 1};
  if (!is_obstacle(node_neighbor)) nodes.push_back(move(node_neighbor));
  node_neighbor = {x - 1, y};
  if (!is_obstacle(node_neighbor)) nodes.push_back(move(node_neighbor));
  node_neighbor = {x + 1, y};
  if (!is_obstacle(node_neighbor)) nodes.push_back(move(node_neighbor));
  node_neighbor = {x, y + 1};
  if (!is_obstacle(node_neighbor)) nodes.push_back(move(node_neighbor));
}

void Map::update_map(const vector<Agent>& agent_list) {
  for (auto const& agent : agent_list) {
    const auto& node = agent.get_current_node();
    map_data[node.x][node.y] = agent.get_name();
  }
}

Map::~Map() {
  if (map_data != nullptr) {
    for (auto i = 0; i < height; i++) {
      if (map_data[i] != nullptr) {
        delete[] map_data[i];
      }
    }
    delete[] map_data;
  }
}
