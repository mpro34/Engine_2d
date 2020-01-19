#ifndef PROJECTILEEMITTERCOMPONENT_H_
#define PROJECTILEEMITTERCOMPONENT_H_

#include "../../lib/glm/glm.hpp"
#include "../EntityManager.h"
#include "TransformComponent.h"

class ProjectileEmitterComponent : public Component {
  private:
    TransformComponent *transform;
    glm::vec2 origin;
    int speed;
    int range;
    float angle_rad; // in radians
    bool should_loop; // destroy projectile or keep repeating?

  public:
    ProjectileEmitterComponent(int speed, int angle_deg, int range, bool should_loop) {
      this->speed = speed;
      this->range = range;
      this->should_loop = should_loop;
      this->angle_rad = glm::radians(static_cast<float>(angle_deg));
    }

    void Initialize() override {
      transform = owner->GetComponent<TransformComponent>();
      origin = glm::vec2(transform->position.x, transform->position.y);
      transform->velocity = glm::vec2(glm::cos(angle_rad) * speed, glm::sin(angle_rad) * speed);
    }

    void Update(float delta_time) override {
      if (glm::distance(transform->position, origin) > range) {
        if (should_loop) {
          transform->position.x = origin.x;
          transform->position.y = origin.y;
        } else {
          owner->Destroy();
        }
      }
    }
};

#endif // PROJECTILEEMITTERCOMPONENT_H_