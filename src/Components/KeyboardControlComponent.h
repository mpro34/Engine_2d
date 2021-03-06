#ifndef KEYBOARDCONTROLCOMPONENT_H_
#define KEYBOARDCONTROLCOMPONENT_H_

#pragma once

#include "../Game.h"
#include "../EntityManager.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"

class KeyboardControlComponent : public Component {
  public:
    std::string upKey;
    std::string downKey;
    std::string rightKey;
    std::string leftKey;
    std::string cKey;  // For collider debug mode
    std::string shootKey;
    TransformComponent *transform;
    SpriteComponent *sprite;

    KeyboardControlComponent() {

    }

    KeyboardControlComponent(std::string upKey, std::string rightKey, std::string downKey, std::string leftKey, std::string cKey, std::string shootKey) {
      this->upKey = GetSDLKeyStringCode(upKey);
      this->rightKey = GetSDLKeyStringCode(rightKey);
      this->downKey = GetSDLKeyStringCode(downKey);
      this->leftKey = GetSDLKeyStringCode(leftKey);
      this->cKey = GetSDLKeyStringCode(cKey);
      this->shootKey = GetSDLKeyStringCode(shootKey);
    }

    std::string GetSDLKeyStringCode(std::string key) {
      if (key.compare("up") == 0) return "1073741906";
      if (key.compare("down") == 0) return "1073741905";
      if (key.compare("left") == 0) return "1073741904";
      if (key.compare("right") == 0) return "1073741903";
      if (key.compare("c") == 0) return "99";
      if (key.compare("space") == 0) return "32";
      return std::to_string(static_cast<int>(key[0]));
    }

    void Initialize() override {
      transform = owner->GetComponent<TransformComponent>();
      sprite = owner->GetComponent<SpriteComponent>();
    }

    void Update(float delta_time) override {
        if(Game::event.type == SDL_KEYDOWN) {
          std::string key_code = std::to_string(Game::event.key.keysym.sym);
          
          if (key_code.compare(upKey) == 0) {
            transform->velocity.y = -70;
            transform->velocity.x = 0;
            sprite->Play("UpAnimation");
          }
          if (key_code.compare(rightKey) == 0) {
            transform->velocity.y = 0;
            transform->velocity.x = 70;
            sprite->Play("RightAnimation");
          }
          if (key_code.compare(downKey) == 0) {
            transform->velocity.y = 70;
            transform->velocity.x = 0;
            sprite->Play("DownAnimation");
          }
          if (key_code.compare(leftKey) == 0) {
            transform->velocity.y = 0;
            transform->velocity.x = -70;
            sprite->Play("LeftAnimation");
          }
          if (key_code.compare(cKey) == 0) {   // Toggle the debug mode collider box.
            Game::debug_mode = !Game::debug_mode;
          }
          if (key_code.compare(shootKey) == 0) {
              // TODO: Shoot projectiles when 'shoot' key is pressed
          }
        }

        if (Game::event.type == SDL_KEYUP) {
          std::string key_code = std::to_string(Game::event.key.keysym.sym);

          if (key_code.compare(upKey) == 0) {
            transform->velocity.y = 0;
          }
          if (key_code.compare(rightKey) == 0) {
            transform->velocity.x = 0;
          }
          if (key_code.compare(downKey) == 0) {
            transform->velocity.y = 0;
          }
          if (key_code.compare(leftKey) == 0) {
            transform->velocity.x = 0;
          }
        }
    }

};

#endif // KEYBOARDCONTROLCOMPONENT_H_