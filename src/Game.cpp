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
Entity* mainPlayer = NULL;
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
      if (asset_type.compare("font") == 0) {
        std::string asset_id = asset["id"];
        std::string asset_file = asset["file"];
        int font_size = asset["fontSize"];
        asset_manager->AddFont(asset_id, asset_file.c_str(), font_size);
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

  /* LOADS ENTITIES FROM LUA CONFIG FILE */
  sol::table level_entities = level_data["entities"];
  unsigned int entity_index = 0;
  while (true) {
    std::cout << "entity index = " << entity_index << std::endl;
    sol::optional<sol::table> existing_entity_index_value = level_entities[entity_index];
    if (existing_entity_index_value == sol::nullopt) {
      break;
    } else {
      sol::table entity = level_entities[entity_index];
      std::string entity_name = entity["name"];
      LayerType layer_type = static_cast<LayerType>(static_cast<int>(entity["layer"]));

      // Add new entity
      Entity& my_entity(manager.AddEntity(entity_name, layer_type));
      if (entity_name == "player") {
        mainPlayer = &my_entity;
      }
      
      // Add Transform component
      sol::optional<sol::table> exists_transform_component = entity["components"]["transform"];
      if (exists_transform_component != sol::nullopt) {
        my_entity.AddComponent<TransformComponent>(
          static_cast<int>(entity["components"]["transform"]["position"]["x"]),
          static_cast<int>(entity["components"]["transform"]["position"]["y"]),
          static_cast<int>(entity["components"]["transform"]["velocity"]["x"]),
          static_cast<int>(entity["components"]["transform"]["velocity"]["y"]),
          static_cast<int>(entity["components"]["transform"]["width"]),
          static_cast<int>(entity["components"]["transform"]["height"]),
          static_cast<int>(entity["components"]["transform"]["scale"])
        );
      }
      std::cout << "After transform processing" << std::endl;

      // Add Sprite component
      sol::optional<sol::table> exists_sprite_component = entity["components"]["sprite"];
      if (exists_sprite_component != sol::nullopt) {
        std::string texture_id = entity["components"]["sprite"]["textureAssetId"];
        bool is_animated = entity["components"]["sprite"]["animated"];
        if (is_animated) {
            my_entity.AddComponent<SpriteComponent>(
              texture_id.c_str(),
              static_cast<int>(entity["components"]["sprite"]["frameCount"]),
              static_cast<int>(entity["components"]["sprite"]["animationSpeed"]),
              static_cast<bool>(entity["components"]["sprite"]["hasDirections"]),
              static_cast<bool>(entity["components"]["sprite"]["fixed"])
            );
        } else {
          my_entity.AddComponent<SpriteComponent>(texture_id.c_str(), false);
        }
      }
      std::cout << "After sprite processing" << std::endl;

      // Add KeyboardControlComponent
      sol::optional<sol::table> exists_input_component = entity["components"]["input"];
      if (exists_input_component != sol::nullopt) {
        sol::optional<sol::table> exists_keyboard_component = entity["components"]["input"]["keyboard"];
        if (exists_keyboard_component != sol::nullopt) {
          std::string up_key = entity["components"]["input"]["keyboard"]["up"];
          std::string left_key = entity["components"]["input"]["keyboard"]["left"];
          std::string down_key = entity["components"]["input"]["keyboard"]["down"];
          std::string right_key = entity["components"]["input"]["keyboard"]["right"];
          std::string shoot_key = entity["components"]["input"]["keyboard"]["shoot"];
          my_entity.AddComponent<KeyboardControlComponent>(up_key, right_key, down_key, left_key, "c", shoot_key);
        }
      }
      std::cout << "After keyboard processing" << std::endl;

      // Add ColliderComponent
      sol::optional<sol::table> exists_collider_component = entity["components"]["collider"];
      if (exists_collider_component != sol::nullopt) {
        my_entity.AddComponent<ColliderComponent>(
          entity["components"]["collider"]["tag"],
          static_cast<int>(entity["components"]["transform"]["position"]["x"]),
          static_cast<int>(entity["components"]["transform"]["position"]["y"]),
          static_cast<int>(entity["components"]["transform"]["width"]),
          static_cast<int>(entity["components"]["transform"]["height"])
        );
      }
      std::cout << "After collider processing" << std::endl;

      // Add Projectile Emitter Component - Need to add Transform, Sprite, Collider and ProjectileEmitter
      sol::optional<sol::table> exists_projectile_component = entity["components"]["projectileEmitter"];
      if (exists_projectile_component != sol::nullopt) {
        int parent_x_pos = entity["components"]["transform"]["position"]["x"];
        int parent_y_pos = entity["components"]["transform"]["position"]["y"];
        int parent_w = entity["components"]["transform"]["width"];
        int parent_h = entity["components"]["transform"]["height"];
        int projectile_w = entity["components"]["projectileEmitter"]["width"];
        int projectile_h = entity["components"]["projectileEmitter"]["height"];
        int projectile_speed = entity["components"]["projectileEmitter"]["speed"];
        int projectile_range = entity["components"]["projectileEmitter"]["range"];
        int projectile_angle = entity["components"]["projectileEmitter"]["angle"];
        bool projectile_should_loop = entity["components"]["projectileEmitter"]["shouldLoop"];
        std::string texture_asset_id = entity["components"]["projectileEmitter"]["textureAssetId"];
        
        Entity& projectile(manager.AddEntity("projectile", PROJECTILE_LAYER));
        projectile.AddComponent<TransformComponent>(
          parent_x_pos + (parent_w / 2),
          parent_y_pos + (parent_h / 2), 
          0, 
          0,
          projectile_w,
          projectile_h,
          1
        );
        projectile.AddComponent<SpriteComponent>(texture_asset_id.c_str());
        projectile.AddComponent<ColliderComponent>(
          "PROJECTILE",
          parent_x_pos,
          parent_y_pos,
          projectile_w,
          projectile_h
        );
        projectile.AddComponent<ProjectileEmitterComponent>(
          projectile_speed,
          projectile_angle,
          projectile_range,
          projectile_should_loop
        );
        std::cout << "After projectile emitter processing" << std::endl;
      }
    }    
    entity_index++;
  }


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
    if (mainPlayer) {
        TransformComponent* mainPlayerTransform = mainPlayer->GetComponent<TransformComponent>();
        camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
        camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

        camera.x = camera.x < 0 ? 0 : camera.x;
        camera.y = camera.y < 0 ? 0 : camera.y;
        camera.x = camera.x > camera.w ? camera.w : camera.x;
        camera.y = camera.y > camera.h ? camera.h : camera.y;
    }
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

