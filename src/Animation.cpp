#include "Animation.h"

Animation::Animation() {
  
}

Animation::Animation(unsigned int index, unsigned int num_frames, unsigned int animation_speed) {
  this->index = index;
  this->num_frames = num_frames;
  this->animation_speed = animation_speed;
}