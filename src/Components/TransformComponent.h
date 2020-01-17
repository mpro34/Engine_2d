#ifndef TRANSFORMCOMPONENT_H_
#define TRANSFORMCOMPONENT_H_

#pragma once

#include "../EntityManager.h"
#include "../Constants.h"
#include "../../lib/glm/glm.hpp"
#include "../Game.h"

class TransformComponent : public Component {
  public:
    glm::vec2 position;
    glm::vec2 velocity;
    int width;
    int height;
    int scale;

    TransformComponent(int pos_x, int pos_y, int vel_x, int vel_y, int w, int h, int s) {
      position = glm::vec2(pos_x, pos_y);
      velocity = glm::vec2(vel_x, vel_y);
      width = w;
      height = h;
      scale = s;
    }

    void Initialize() override {

    }

    void Update(float delta_time) override {
      position.x += velocity.x * delta_time;
      if (position.x > WINDOW_WIDTH-width) {  // clamp
        position.x = WINDOW_WIDTH-width;
      } else if (position.x < 0) {
        position.x = 0;
      }

      position.y += velocity.y * delta_time;
      if (position.y > WINDOW_HEIGHT-height) {  // clamp
        position.y = WINDOW_HEIGHT-height;
      } else if (position.y < 0) {
        position.y = 0;
      }
    }

    void Render() override {
      
    }

};

#endif // TRANSFORMCOMPONENT_H_