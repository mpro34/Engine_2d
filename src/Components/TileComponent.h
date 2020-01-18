#ifndef TILECOMPONENT_H_
#define TILECOMPONENT_H_

#pragma once

#include <SDL2/SDL.h>
#include "../EntityManager.h"
#include "../AssetManager.h"
#include "../../lib/glm/glm.hpp"

class TileComponent : public Component {
  public:
    SDL_Texture *texture;
    SDL_Rect source_rect;
    SDL_Rect dest_rect;
    glm::vec2 position;

    TileComponent(int source_rect_x, int source_rect_y, int x, int y, int tile_size, int tile_scale, std::string asset_texture_id) {
      texture = Game::asset_manager->GetTexture(asset_texture_id);
      source_rect.x = source_rect_x;
      source_rect.y = source_rect_y;
      source_rect.w = tile_size;
      source_rect.h = tile_size;

      dest_rect.x = x;
      dest_rect.y = y;
      dest_rect.w = tile_size * tile_scale;
      dest_rect.h = tile_size * tile_scale;

      position.x = x;
      position.y = y;
    }

    ~TileComponent() {
      SDL_DestroyTexture(texture);
    }

    void Update(float delta_time) override {
      // Take care of the tile positions based on camera control
      dest_rect.x = position.x - Game::camera.x;
      dest_rect.y = position.y - Game::camera.y;
    }

    void Render() override {
      TextureManager::Draw(texture, source_rect, dest_rect, SDL_FLIP_NONE);
    }
};

#endif // TILECOMPONENT_H_
