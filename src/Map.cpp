#include <fstream>
#include "Game.h"
#include "Map.h"
#include "EntityManager.h"
#include "Components/TileComponent.h"

extern EntityManager manager;

Map::Map(std::string texture_id, int scale, int tile_size) {
  this->texture_id = texture_id;
  this->scale = scale;
  this->tile_size = tile_size;
}

void Map::LoadMap(std::string file_path, int map_size_x, int map_size_y) {
  // Read the map tile definitions from the .map file
  // The first digit is the y-axis in the .map file and the second digit is the x-axis.
  std::fstream map_file;
  map_file.open(file_path);

  for (int y=0; y < map_size_y; y++) {
    for (int x=0; x < map_size_x; x++) {
      char ch;
      map_file.get(ch);
      int source_rect_y = atoi(&ch) * tile_size;
      map_file.get(ch);
      int source_rect_x = atoi(&ch) * tile_size;
      AddTile(source_rect_x, source_rect_y, x * (scale * tile_size), y * (scale * tile_size));
      map_file.ignore();  // Ignore the rest of the file
    }
  }
  map_file.close();
}

void Map::AddTile(int source_rect_x, int source_rect_y, int x, int y) {
  // Add a new tile entity in the game scene.
  Entity& new_tile(manager.AddEntity("Tile"));
  new_tile.AddComponent<TileComponent>(source_rect_x, source_rect_y, x, y, tile_size, scale, texture_id);
}