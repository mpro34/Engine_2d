#ifndef TRANSFORMCOMPONENT_H_
#define TRANSFORMCOMPONENT_H_

#include <SDL2/SDL.h>
#include "../EntityManager.h"
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
      position.y += velocity.y * delta_time;
    }

    void Render() override {
      SDL_Rect transformRectangle = {
        (int) position.x,
        (int) position.y,
        width, 
        height
      };
      
      SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
      SDL_RenderFillRect(Game::renderer, &transformRectangle);

    }

};

#endif // TRANSFORMCOMPONENT_H_