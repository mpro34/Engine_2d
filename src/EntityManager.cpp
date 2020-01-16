#include "EntityManager.h"

void EntityManager::ClearData() {
  for (auto& entity : entities) {
    entity->Destroy();
  }
}

bool EntityManager::HasNoEntities() {
  return entities.size() == 0;
}

void EntityManager::Update(float delta_time) {
  for (auto& entity : entities) {
    entity->Update(delta_time);
  }
}

void EntityManager::Render() {
  for (auto& entity : entities) {
    entity->Render();
  }
}

Entity& EntityManager::AddEntity(std::string entity_name) {
  Entity* entity = new Entity(*this, entity_name);
  entities.emplace_back(entity);
  return *entity;
}

std::vector<Entity*> EntityManager::GetEntities() const {
  return entities;
}

unsigned int EntityManager::GetEntityCount() {
  return entities.size();
}