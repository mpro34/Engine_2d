#ifndef SPRITECOMPONENT_H_
#define SPRITECOMPONENT_H_

#pragma once

#include <SDL2/SDL.h>
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "../Animation.h"

class SpriteComponent : public Component {
  private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect source_rect;
    SDL_Rect dest_rect;
    bool is_animated;
    int num_frames;
    int animation_speed;
    bool is_fixed;
    std::map<std::string, Animation> animations;
    std::string current_animation_name;
    unsigned int animation_index = 0;

  public:
    SDL_RendererFlip sprite_flip = SDL_FLIP_NONE;

    SpriteComponent(const char* asset_texture_id) {
      is_animated = false;
      is_fixed = false;
      SetTexture(asset_texture_id);
    }

    void SetTexture(std::string asset_texture_id) {
      texture = Game::asset_manager->GetTexture(asset_texture_id);
    }

    SpriteComponent(std::string id, int num_frames, int animation_speed, bool has_directions, bool is_fixed) {
      is_animated = true;
      this->num_frames = num_frames;
      this->animation_speed = animation_speed;
      this->is_fixed = is_fixed;

      if (has_directions) {
        Animation down_animation = Animation(0, num_frames, animation_speed);
        Animation right_animation = Animation(1, num_frames, animation_speed);
        Animation left_animation = Animation(2, num_frames, animation_speed);
        Animation up_animation = Animation(3, num_frames, animation_speed);
        animations.emplace("DownAnimation", down_animation);
        animations.emplace("RightAnimation", right_animation);
        animations.emplace("LeftAnimation", left_animation);
        animations.emplace("UpAnimation", up_animation);
        this->animation_index = 0;
        this->current_animation_name = "DownAnimation";

      } else {
        Animation single_animation = Animation(0, num_frames, animation_speed);
        animations.emplace("SingleAnimation", single_animation);
        this->animation_index = 0;
        this->current_animation_name = "SingleAnimation";
      }
      Play(this->current_animation_name);
      SetTexture(id);
    }

    void Play(std::string animation_name) {
      num_frames = animations[animation_name].num_frames;
      animation_index = animations[animation_name].index;
      animation_speed = animations[animation_name].animation_speed;
      current_animation_name = animation_name;
    }

    void Initialize() override {
      transform = owner->GetComponent<TransformComponent>();
      source_rect.x = 0;
      source_rect.y = 0;
      source_rect.w = transform->width;
      source_rect.h = transform->height;
    }
    
    void Update(float delta_time) override {
      if (is_animated) {
        source_rect.x = source_rect.w * static_cast<int>((SDL_GetTicks() / animation_speed) % num_frames);
      }
      source_rect.y = animation_index * transform->height;

      dest_rect.x = static_cast<int>(transform->position.x);
      dest_rect.y = static_cast<int>(transform->position.y);
      dest_rect.w = transform->width * transform->scale;
      dest_rect.h = transform->height * transform->scale;
    }

    void Render() override {
      TextureManager::Draw(texture, source_rect, dest_rect, sprite_flip);
    }

};

#endif // SPRITECOMPONENT_H_