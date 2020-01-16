#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Game {
  private:
    bool isRunning_;
    SDL_Window *window;

  public:
    Game();
    ~Game();
    int ticks_last_frame;
    bool IsRunning() const;
    static SDL_Renderer *renderer;
    void LoadLevel(int levelNumber);
    void Initialize(int width, int height);
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();
};

#endif // GAME_H_