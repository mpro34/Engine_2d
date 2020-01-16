#ifndef ENTITYMANAGER_H_
#define ENTITYMANAGER_H_

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
    Entity& AddEntity(std::string entityName);
    std::vector<Entity*> GetEntities() const;
    unsigned int GetEntityCount();
};

#endif // ENTITYMANAGER_H_