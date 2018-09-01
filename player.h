#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

struct Player {
  int posX, posY;
  int width, height;
  SDL_Rect rect;
};

struct Player* create_player(SDL_Renderer* gRenderer);
void move_player(struct Player* player);
#endif
