#ifndef FONTMANAGER_H_
#define FONTMANAGER_H_

#include "Game.h"

class FontManager {
  public:
    static TTF_Font* LoadFont(const char* file_name, int font_size);
    static void Draw(SDL_Texture* texture, SDL_Rect position);
};

#endif // FONTMANAGER_H_