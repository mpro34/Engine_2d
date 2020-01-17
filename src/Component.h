#ifndef COMPONENT_H_
#define COMPONENT_H_

#pragma once

class Entity;

class Component {
  public:
    Entity* owner;
    virtual ~Component() {};
    virtual void Initialize() {};
    virtual void Update(float delta_time) {};
    virtual void Render() {};

};

#endif // COMPONENT_H_