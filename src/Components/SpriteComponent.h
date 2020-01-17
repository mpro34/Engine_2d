#ifndef SPRITECOMPONENT_H_
#define SPRITECOMPONENT_H_

#include <SDL2/SDL.h>
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "TransformComponent.h"

class SpriteComponent : public Component {
  private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect source_rect;
    SDL_Rect dest_rect;

  public:
    SDL_RendererFlip sprite_flip = SDL_FLIP_NONE;

    SpriteComponent(const char* file_path) {
      SetTexture(file_path);
    }

    void SetTexture(std::string asset_texture_id) {
      texture = Game::asset_manager->GetTexture(asset_texture_id);
    }

    void Initialize() override {
      transform = owner->GetComponent<TransformComponent>();
      source_rect.x = 0;
      source_rect.y = 0;
      source_rect.w = transform->width;
      source_rect.h = transform->height;
    }
    
    void Update(float delta_time) override {
      dest_rect.x = (int) transform->position.x;
      dest_rect.y = (int) transform->position.y;
      dest_rect.w = (int) transform->width * transform->scale;
      dest_rect.h = (int) transform->height * transform->scale;
    }

    void Render() override {
      TextureManager::Draw(texture, source_rect, dest_rect, sprite_flip);
    }

};

#endif // SPRITECOMPONENT_H_