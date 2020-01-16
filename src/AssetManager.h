#ifndef ASSETMANAGER_H_
#define ASSETMANAGER_H_

#include <string>
#include <map>
#include "TextureManager.h"
#include "EntityManager.h"

class AssetManager {
  private:
    EntityManager* manager;
    std::map<std::string, SDL_Texture*> textures;

  public:
    AssetManager(EntityManager* manager);
    ~AssetManager();
    void ClearData();
    void AddTexture(std::string textureId, const char* file_path);
    SDL_Texture* GetTexture(std::string textureId);
};

#endif // ASSETMANAGER_H_