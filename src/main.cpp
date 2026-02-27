#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <iostream>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

using namespace std;

SDL_Window *window = NULL;
SDL_Surface *windowSurface = NULL;
SDL_Renderer *renderer = NULL;

bool init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    cout << SDL_GetError() << endl;

    return false;
  }

  window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_SHOWN);

  if (window != NULL) {
    cout << SDL_GetError() << endl;

    return false;
  }

  windowSurface = SDL_GetWindowSurface(window);

  if (windowSurface == NULL) {
    cout << SDL_GetError() << endl;

    return false;
  }

  renderer = SDL_GetRenderer(window);

  return true;
}

bool loadMedia() {
  return true;
}

void close() {

  bool quit = false;

  while (!quit) {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) quit = true;
    }
  }

  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer);
  SDL_DestroyWindow(window);
}

int main(int argc, char *args[]) {

  init();
  loadMedia();

  SDL_Rect outlineRect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6,
                          SCREEN_HEIGHT * 2 / 3, SCREEN_HEIGHT * 2 / 3};
  SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
  SDL_RenderDrawRect(renderer, &outlineRect);

  close();
  return 0;
}
