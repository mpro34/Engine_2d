#include <iostream>
#include "Constants.h"
#include "Game.h"
#include "Components/TransformComponent.h"
#include "../lib/glm/glm.hpp"

class EntityManager;

EntityManager manager;
SDL_Renderer* Game::renderer;

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

  this->isRunning_ = true;
  return;
}

void Game::LoadLevel(int level_number) {
  Entity& new_entity(manager.AddEntity("projectile"));
  new_entity.AddComponent<TransformComponent>(0, 0, 20, 20, 32, 32, 1);
}

void Game::ProcessInput() {
  SDL_Event event;
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

void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

