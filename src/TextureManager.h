#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#pragma once

#include "Game.h"

class TextureManager {
  public:
    static SDL_Texture* LoadTexture(const char* file_name);
    static void Draw(SDL_Texture* texture, SDL_Rect source_rect, SDL_Rect dest_rect, SDL_RendererFlip flip);
};

#endif // TEXTUREMANAGER_H_