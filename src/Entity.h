#ifndef ENTITY_H_
#define ENTITY_H_

#pragma once

#include <vector>
#include <string>
#include <map>
#include "Component.h"
#include "Constants.h"
#include "EntityManager.h"

class EntityManager;
class Component;

class Entity {
  private:
    EntityManager& manager;
    bool isActive;
    std::vector<Component*> components;
    std::map<const std::type_info*, Component*> component_type_map;

  public:
    std::string name;
    LayerType layer;
    Entity(EntityManager& manager);
    Entity(EntityManager& manager, std::string name, LayerType layer);
    void Update(float delta_time);
    void Render();
    void Destroy();
    bool IsActive() const;
    void ListAllComponents() const;

    template <typename T, typename... TArgs>
    T& AddComponent(TArgs&&... args) {
      T* new_component(new T(std::forward<TArgs>(args)...));
      new_component->owner = this;
      components.emplace_back(new_component);
      component_type_map[&typeid(*new_component)] = new_component;
      new_component->Initialize();
      return *new_component;
    }

    template <typename T>
    T* GetComponent() {
      return static_cast<T*>(component_type_map[&typeid(T)]);
    }

    template <typename T>
    bool HasComponent() const {
      return component_type_map.count(&typeid(T));
    }
};

#endif // ENTITY_H_