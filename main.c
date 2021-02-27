#include <SDL2/SDL.h>
#include "functions.c"
int main(int argc, char * argv[]) {
  SDL_Window * window = SDL_CreateWindow("Maze", 20, 20, 800, 800, SDL_WINDOW_OPENGL);
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer, 134, 47, 47, SDL_ALPHA_OPAQUE);
  SDL_Rect rectangle;
  SDL_Event event;
  SDL_bool program_launched = SDL_TRUE;
  while (program_launched) {
    while (SDL_PollEvent( & event)) {
      if (event.type == SDL_QUIT)
        program_launched = SDL_FALSE;
    }
    generate();
    show();
    print(renderer, rectangle);
    clear(renderer, rectangle);
    solution(M - 1, 1, renderer, rectangle);
}
}
