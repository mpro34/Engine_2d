#include "AssetManager.h"

AssetManager::AssetManager(EntityManager* manager): manager(manager) {}

void AssetManager::ClearData() {
  textures.clear();
  fonts.clear();
}

void AssetManager::AddTexture(std::string textureId, const char* file_path) {
  textures.emplace(textureId, TextureManager::LoadTexture(file_path));
}

void AssetManager::AddFont(std::string font_id, const char* file_path, int font_size) {
  fonts.emplace(font_id, FontManager::LoadFont(file_path, font_size));
}

SDL_Texture* AssetManager::GetTexture(std::string textureId) {
  return textures[textureId];
}

TTF_Font* AssetManager::GetFont(std::string font_id) {
  return fonts[font_id];
}