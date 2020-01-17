#ifndef ANIMATION_H_
#define ANIMATION_H_

#pragma once

class Animation {
  public:
    unsigned int index;
    unsigned int num_frames;
    unsigned int animation_speed;
    Animation();
    Animation(unsigned int index, unsigned int num_frames, unsigned int animation_speed);
};

#endif // ANIMATION_H_