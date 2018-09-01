#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#include "player.h"

struct Player* create_player( SDL_Renderer* gRenderer ) {
  struct Player* p = malloc(sizeof(struct Player));
  p->posX = 0;
  p->posY = 100;
  p->width = 64 * 3;
  p->height = 64 * 3;

  SDL_Rect r = { .x = p->posX, .y = p->posY, .w = p->width, .h = p->height};
  p->rect = r;

  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

  SDL_RenderFillRect( gRenderer, &p->rect );

  return p;
}

void move_player(struct Player* player) {
  printf("GOO\n");
    const Uint8* key_state = SDL_GetKeyboardState( NULL );
    if( key_state[ SDL_SCANCODE_D ] ) {
          player->posX += 10;
    }
    if( key_state[ SDL_SCANCODE_A ] ) {
          player->posX -= 10;
      }
    if( key_state[ SDL_SCANCODE_W ] ) {
      player->posY -= 10;
    }
    if( key_state[ SDL_SCANCODE_S ] ) {
      player->posY += 10;
    }
}
