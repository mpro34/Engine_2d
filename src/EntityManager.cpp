#include <iostream>
#include "EntityManager.h"
#include "Collision.h"
#include "Components/ColliderComponent.h"

void EntityManager::ClearData() {
  for (auto& entity : entities) {
    entity->Destroy();
  }
}

bool EntityManager::HasNoEntities() {
  return entities.size() == 0;
}

void EntityManager::Update(float delta_time) {
  for (int i=0; i<entities.size(); i++) {
    if (entities[i]->IsActive()) {
      entities[i]->Update(delta_time);
    } else {
      entities.erase(entities.begin() + i);
    }
    
  }
}

void EntityManager::Render() {
  for (int layer_num=0; layer_num < NUM_LAYERS; layer_num++) {
    for (auto& entity : GetEntitiesByLayer(static_cast<LayerType>(layer_num))) {
      entity->Render();
    }
  }
}

Entity& EntityManager::AddEntity(std::string entity_name, LayerType layer) {
  Entity* entity = new Entity(*this, entity_name, layer);
  entities.emplace_back(entity);
  return *entity;
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const {
  std::vector<Entity*> selected_entities;
  for (auto& entity : entities) {
    if (entity->layer == layer) {
      selected_entities.emplace_back(entity);
    }
  }
  return selected_entities;
}

std::vector<Entity*> EntityManager::GetEntities() const {
  return entities;
}

unsigned int EntityManager::GetEntityCount() {
  return entities.size();
}

void EntityManager::ListAllEntities() const {
  unsigned int i = 0;
  for (auto& entity : entities) {
    std::cout << "Entity[" << i << "]:" << entity->name << std::endl;
    entity->ListAllComponents();
    i++;
  }
}

CollisionType EntityManager::CheckCollisions() const {
  for (auto& this_entity : entities) {
    if (this_entity->HasComponent<ColliderComponent>()) {
      ColliderComponent* this_collider = this_entity->GetComponent<ColliderComponent>();
      for (auto& that_entity : entities) {
        if (this_entity->name.compare(that_entity->name) != 0 && that_entity->HasComponent<ColliderComponent>()) {
          ColliderComponent* that_collider = that_entity->GetComponent<ColliderComponent>();
          if (Collision::CheckRectangleCollision(this_collider->collider, that_collider->collider)) {
            if (
              this_collider->collider_tag.compare("PLAYER") == 0 &&
              that_collider->collider_tag.compare("ENEMY") == 0
            ) {
              return PLAYER_ENEMY_COLLISION;
            }

            if (
              this_collider->collider_tag.compare("PLAYER") == 0 &&
              that_collider->collider_tag.compare("PROJECTILE") == 0
            ) {
              return PLAYER_PROJECTILE_COLLISION;
            }

            if (
              this_collider->collider_tag.compare("ENEMY") == 0 &&
              that_collider->collider_tag.compare("FRIENDLY_PROJECTILE") == 0
            ) {
              return ENEMY_PROJECTILE_COLLISION;
            }

            if (
              this_collider->collider_tag.compare("PLAYER") == 0 &&
              that_collider->collider_tag.compare("LEVEL_COMPLETE") == 0
            ) {
              return PLAYER_LEVEL_COMPLETE_COLLISION;
            }

          }
        }
      }
    }
  }
  return NO_COLLISION;
  // ColliderComponent* my_collider = my_entity.GetComponent<ColliderComponent>();
  // for (auto& entity : entities) {
  //   if (entity->name.compare(my_entity.name) != 0 && entity->name.compare("Tile")) {
  //     if (entity->HasComponent<ColliderComponent>()) {
  //       ColliderComponent* other_collider = entity->GetComponent<ColliderComponent>();
  //       if (Collision::CheckRectangleCollision(my_collider->collider, other_collider->collider)) {
  //         return other_collider->collider_tag;
  //       }
  //     }
  //   }
  // }
  // return std::string();
}