#ifndef MAP_H_
#define MAP_H_

#pragma once

#include <string>

class Map {
  private:
    std::string texture_id;
    int scale;
    int tile_size;

  public:
    Map(std::string texture_id, int scale, int tile_size);
    ~Map();
    void LoadMap(std::string file_path, int map_size_x, int map_size_y);
    void AddTile(int source_x, int source_y, int x, int y);

};

#endif // MAP_H_