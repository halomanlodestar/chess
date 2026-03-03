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

const int WIDHT = 1000;
const int HEIGHT = 1000;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

bool init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    cout << SDL_GetError() << endl;

    return false;
  }

  window = SDL_CreateWindow("Chess Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDHT, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (window == nullptr) {
    cout << SDL_GetError() << endl;

    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_WINDOW_SHOWN);

  if (renderer == nullptr) {
    cout << SDL_GetError() << endl;

    SDL_DestroyWindow(window);

    return false;
  }

  return true;
}

void render() {}

void eventLoop() {

  bool running = true;

  while (running) {
    SDL_Event e;

    while () {}
  }

}

void close() {}

int main() {
  return 0;
}