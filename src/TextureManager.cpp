#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* file_name) {
  SDL_Surface* surface = IMG_Load(file_name);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
  SDL_FreeSurface(surface);
  return texture;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect source_rect, SDL_Rect dest_rect, SDL_RendererFlip flip) {
  SDL_RenderCopyEx(Game::renderer, texture, &source_rect, &dest_rect, 0.0, NULL, flip);
}