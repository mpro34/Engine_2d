#ifndef GAME_H_
#define GAME_H_

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Entity.h"
#include "Component.h"
#include "EntityManager.h"

class AssetManager;

class Game {
  private:
    bool isRunning_;
    SDL_Window *window;

  public:
    Game();
    ~Game();
    int ticks_last_frame;
    bool IsRunning() const;
    static SDL_Renderer *renderer;
    static AssetManager* asset_manager;
    static SDL_Event event;
    static SDL_Rect camera;
    void LoadLevel(int levelNumber);
    void Initialize(int width, int height);
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();
    void HandleCameraMovement();
    void CheckCollisions();
};

#endif // GAME_H_