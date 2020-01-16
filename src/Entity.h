#ifndef ENTITY_H_
#define ENTITY_H_

#include <vector>
#include <string>
#include "Component.h"
#include "EntityManager.h"

class EntityManager;
class Component;

class Entity {
  private:
    EntityManager& manager;
    bool isActive;
    std::vector<Component*> components;

  public:
    std::string name;
    Entity(EntityManager& manager);
    Entity(EntityManager& manager, std::string name);
    void Update(float delta_time);
    void Render();
    void Destroy();
    bool IsActive() const;
};

#endif // ENTITY_H_