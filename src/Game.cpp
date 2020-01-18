#include <iostream>
#include "Constants.h"
#include "Game.h"
#include "AssetManager.h"
#include "Map.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/KeyboardControlComponent.h"
#include "../lib/glm/glm.hpp"

class EntityManager;

EntityManager manager;
AssetManager* Game::asset_manager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map *map;

Game::Game() {
  this->isRunning_ = false;
}

Game::~Game() {
}

bool Game::IsRunning() const {
  return this->isRunning_;
}

void Game::Initialize(int width, int height) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr << "Error initializing SDL." << std::endl;
    return;
  }
  // Create the SDL window
  window = SDL_CreateWindow(
    NULL,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    width,
    height,
    SDL_WINDOW_BORDERLESS
  );
  if (!window) {
    std::cerr << "Error creating SDL window." << std::endl;
    return;
  }
  // Create the SDL renderer
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    std::cerr << "Error creating SDL renderer." << std::endl;
    return;
  }

  LoadLevel(0);

  //manager.ListAllEntities();  // Used for debugging entities

  this->isRunning_ = true;
  return;
}

Entity& player(manager.AddEntity("chopper", PLAYER_LAYER));

void Game::LoadLevel(int level_number) {
  /* Start including new assets to the assetmanager */
  asset_manager->AddTexture("tank-image", std::string("assets/images/tank-big-right.png").c_str());
  asset_manager->AddTexture("chopper-image", std::string("assets/images/chopper-spritesheet.png").c_str());
  asset_manager->AddTexture("radar-image", std::string("assets/images/radar.png").c_str());
  asset_manager->AddTexture("jungle-tile-texture", std::string("assets/tilemaps/jungle.png").c_str());

  map = new Map("jungle-tile-texture", 2, 32);
  map->LoadMap("assets/tilemaps/jungle.map", 25, 20);  // Path and size of the .map file.

  player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
  player.AddComponent<SpriteComponent>("chopper-image", 2, 150, true, false);
  player.AddComponent<KeyboardControlComponent>("up", "right", "down", "left", "space");

  /* Start including entities and also components to them */
  Entity& tankEntity(manager.AddEntity("tank", ENEMY_LAYER));
  tankEntity.AddComponent<TransformComponent>(125, 500, 20, 0, 32, 32, 1);
  tankEntity.AddComponent<SpriteComponent>("tank-image");

  Entity& radarEntity(manager.AddEntity("Radar", UI_LAYER));
  radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
  radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);
}

void Game::ProcessInput() {
  SDL_PollEvent(&event);
  switch (event.type) {
    case SDL_QUIT: {
      this->isRunning_ = false;
      break;
    }
    case SDL_KEYDOWN: {
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        this->isRunning_ = false;
        break;
      }
    }
    default: {
      break;
    }
  }
}

void Game::Update() {
  // Wait until 16ms (60/1000) has ellapsed since the last frame
  while(!SDL_TICKS_PASSED(SDL_GetTicks(), ticks_last_frame + FRAME_TARGET_TIME));

  // delta_time is the difference in ticks from last frame converted to seconds
  float delta_time = (SDL_GetTicks() - ticks_last_frame) / 1000.0f;

  // Clamp delta_time to a max value
  delta_time = (delta_time > 0.05f) ? 0.05f : delta_time;

  // Sets the new ticks for the current frame to be used in the next pass
  ticks_last_frame = SDL_GetTicks();

  manager.Update(delta_time);

  HandleCameraMovement();

}

void Game::Render() {
  // set the background color
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  // clear the back buffer
  SDL_RenderClear(renderer); 

  /* Draw all game objects of the scene*/
  if (manager.HasNoEntities()) return;

  manager.Render();

  SDL_RenderPresent(renderer);
}

void Game::HandleCameraMovement() {
  TransformComponent* main_player_trans = player.GetComponent<TransformComponent>();

  camera.x = main_player_trans->position.x - (WINDOW_WIDTH / 2);
  camera.y = main_player_trans->position.y - (WINDOW_HEIGHT / 2);

  camera.x = camera.x < 0 ? 0 : camera.x;
  camera.y = camera.y < 0 ? 0 : camera.y;
  camera.x = camera.x > camera.w ? camera.w : camera.x;
  camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

