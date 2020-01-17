#include <iostream>
#include "Entity.h"

Entity::Entity(EntityManager& manager): manager(manager) {
  this->isActive = true;
};

Entity::Entity(EntityManager& manager, std::string name): manager(manager), name(name) {
  this->isActive = true;
}

void Entity::Update(float delta_time) {
  for (auto& component : components) {
    component->Update(delta_time);
  }
}

void Entity::Render() {
  for (auto& component : components) {
    component->Render();
  }
}

void Entity::Destroy() {
  this->isActive = false;
}

bool Entity::IsActive() const {
  return this->isActive;
}

void Entity::ListAllComponents() const {
  for (auto& map_element : component_type_map) {
    std::cout << "  Component<" << map_element.first->name() << ">" << std::endl;
  }
}