#ifndef TEXTLABELCOMPONENT_H_
#define TEXTLABELCOMPONENT_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../FontManager.h"
#include "../EntityManager.h"
#include "../AssetManager.h"
#include "../Game.h"

class TextLabelComponent : public Component {
  private:
    SDL_Rect position;
    std::string text;
    std::string font_family;
    SDL_Color color;
    SDL_Texture* texture;

  public:
    TextLabelComponent(int x, int y, std::string text, std::string font_family, const SDL_Color& color) {
      this->position.x = x;
      this->position.y = y;
      this->text = text;
      this->font_family = font_family;
      this->color = color;
      SetLabelText(text, font_family);
    }

    void SetLabelText(std::string text, std::string font_family) {
      SDL_Surface* surface = TTF_RenderText_Blended(Game::asset_manager->GetFont(font_family), text.c_str(), color);
      texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
      SDL_FreeSurface(surface);
      SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
    }

    void Render() override {
      FontManager::Draw(texture, position);
    }

};

#endif // TEXTLABELCOMPONENT_H_