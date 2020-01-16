#include "AssetManager.h"

AssetManager::AssetManager(EntityManager* manager): manager(manager) {}

void AssetManager::ClearData() {
  textures.clear();
}

void AssetManager::AddTexture(std::string textureId, const char* file_path) {
  textures.emplace(textureId, TextureManager::LoadTexture(file_path));
}

SDL_Texture* AssetManager::GetTexture(std::string textureId) {
  return textures[textureId];
}