#include "FontManager.h"

TTF_Font* FontManager::LoadFont(const char* file_name, int font_size) {
  return TTF_OpenFont(file_name, font_size);
}

void FontManager::Draw(SDL_Texture* texture, SDL_Rect position) {
  SDL_RenderCopy(Game::renderer, texture, NULL, &position);
}