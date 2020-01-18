#ifndef COLLIDERCOMPONENT_H_
#define COLLIDERCOMPONENT_H_

#pragma once

#include "../Game.h"
#include "../EntityManager.h"
#include "TransformComponent.h"
#include <SDL2/SDL.h>

class ColliderComponent : public Component {
  public:
    std::string collider_tag;
    SDL_Rect collider;
    SDL_Rect source_rect;
    SDL_Rect dest_rect;
    TransformComponent* transform;

    ColliderComponent(std::string collider_tag, int x, int y, int width, int height) {
      this->collider_tag = collider_tag;
      this->collider = {x, y, width, height};
    }

    void Initialize() override {
      if (owner->HasComponent<TransformComponent>()) {
        transform = owner->GetComponent<TransformComponent>();
        source_rect = {0, 0, transform->width, transform->height};
        dest_rect = {collider.x, collider.y, collider.w, collider.h};
      }
    }

    void Update(float delta_time) override {
      collider.x = static_cast<int>(transform->position.x);
      collider.y = static_cast<int>(transform->position.y);
      collider.w = transform->width * transform->scale;
      collider.h = transform->height * transform->scale;
      dest_rect.x = collider.x - Game::camera.x;
      dest_rect.y = collider.y - Game::camera.y;
    }

    void Render() override {
      // TODO: Add red bounding box border to show collisions
    }

};

#endif // COLLIDERCOMPONENT_H_