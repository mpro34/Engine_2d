#ifndef ENTITYMANAGER_H_
#define ENTITYMANAGER_H_

#pragma once

#include "Entity.h"
#include "Component.h"
#include <vector>

class EntityManager {
  private:
    std::vector<Entity*> entities;

  public:
    void ClearData();
    void Update(float delta_time);
    void Render();
    bool HasNoEntities();
    Entity& AddEntity(std::string entityName, LayerType layer);
    std::vector<Entity*> GetEntities() const;
    std::vector<Entity*> GetEntitiesByLayer(LayerType layer) const;
    void ListAllEntities() const;
    unsigned int GetEntityCount();
};

#endif // ENTITYMANAGER_H_