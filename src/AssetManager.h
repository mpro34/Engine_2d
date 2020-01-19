#ifndef ASSETMANAGER_H_
#define ASSETMANAGER_H_

#pragma once

#include <string>
#include <map>
#include <SDL2/SDL_ttf.h>
#include "TextureManager.h"
#include "FontManager.h"
#include "EntityManager.h"

class AssetManager {
  private:
    EntityManager* manager;
    std::map<std::string, SDL_Texture*> textures;
    std::map<std::string, TTF_Font*> fonts;

  public:
    AssetManager(EntityManager* manager);
    ~AssetManager();
    void ClearData();

    void AddTexture(std::string textureId, const char* file_path);
    void AddFont(std::string font_id, const char* file_path, int font_size);

    SDL_Texture* GetTexture(std::string textureId);
    TTF_Font* GetFont(std::string font_id);
};

#endif // ASSETMANAGER_H_