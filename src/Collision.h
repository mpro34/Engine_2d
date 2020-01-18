#ifndef COLLISION_H_
#define COLLISION_H_

#pragma once

#include <SDL2/SDL.h>

class Collision {
  public:
    static bool CheckRectangleCollision(const SDL_Rect& rectA, const SDL_Rect& rectB);
};

#endif // COLLISION_H_