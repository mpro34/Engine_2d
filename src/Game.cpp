#include <iostream>
#include "Constants.h"
#include "Game.h"
#include "AssetManager.h"
#include "Map.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/KeyboardControlComponent.h"
#include "Components/TextLabelComponent.h"
#include "Components/ProjectileEmitterComponent.h"
#include "../lib/glm/glm.hpp"

class EntityManager;

EntityManager manager;
AssetManager* Game::asset_manager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
bool Game::debug_mode = false;
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
  if (TTF_Init() != 0) {
    std::cerr << "Error initializing SDL TTF." << std::endl;
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

  LoadLevel(1);

  this->isRunning_ = true;
  return;
}

// Entity& player(manager.AddEntity("chopper", PLAYER_LAYER));

void Game::LoadLevel(int level_number) {
  // Load level(s) in by lua scripts with help of sol
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

  std::string level_name = "Level" + std::to_string(level_number);
  lua.script_file("assets/scripts/" + level_name + ".lua");
  sol::table level_data = lua[level_name];
  
  /* LOADS ASSETS FROM LUA CONFIG FILE */
  sol::table level_assets = level_data["assets"];
  unsigned int asset_index = 0;
  while (true) {
    sol::optional<sol::table> existing_asset_index_value = level_assets[asset_index];
    if (existing_asset_index_value == sol::nullopt) {
      break;
    } else {
      sol::table asset = level_assets[asset_index];
      std::string asset_type = asset["type"];
      if (asset_type.compare("texture") == 0) {
        std::string asset_id = asset["id"];
        std::string asset_file = asset["file"];
        asset_manager->AddTexture(asset_id, asset_file.c_str());
      }
    }    
    asset_index++;
  }

  /* LOADS MAP FROM LUA CONFIG FILE */
  sol::table level_map = level_data["map"];
  std::string map_texture_id = level_map["textureAssetId"];
  std::string map_file = level_map["file"];
  map = new Map(
    map_texture_id,
    static_cast<int>(level_map["scale"]),
    static_cast<int>(level_map["tileSize"])
  );
  map->LoadMap(
    map_file, 
    static_cast<int>(level_map["mapSizeX"]),
    static_cast<int>(level_map["mapSizeY"])
  );

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
  CheckCollisions();
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
  // TransformComponent* main_player_trans = player.GetComponent<TransformComponent>();

  // camera.x = main_player_trans->position.x - (WINDOW_WIDTH / 2);
  // camera.y = main_player_trans->position.y - (WINDOW_HEIGHT / 2);

  // camera.x = camera.x < 0 ? 0 : camera.x;
  // camera.y = camera.y < 0 ? 0 : camera.y;
  // camera.x = camera.x > camera.w ? camera.w : camera.x;
  // camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::CheckCollisions() {
  CollisionType collision_type = manager.CheckCollisions();
  if (collision_type == PLAYER_ENEMY_COLLISION) {
    ProcessGameOver();
  }
  if (collision_type == PLAYER_PROJECTILE_COLLISION) {
    ProcessGameOver();
  }
  if (collision_type == PLAYER_LEVEL_COMPLETE_COLLISION) {
    ProcessNextLevel(1);
  }
}

void Game::ProcessNextLevel(int level_num) {
  std::cout << "Next Level" << std::endl;
  this->isRunning_ = false;
}

void Game::ProcessGameOver() {
  std::cout << "Game Over" << std::endl;
  this->isRunning_ = false;
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

