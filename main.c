#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "player.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

enum KeyPresSurfaces {
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL
};
bool init();

bool loadMedia();

void cleanup();

SDL_Texture* loadTexture(char* path);

SDL_Texture* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

SDL_Texture* gTexture = NULL;

SDL_Texture* gFloor = NULL;

SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Texture* loadTexture( char* path) {
  SDL_Texture* newTexture = NULL;

  SDL_Surface* loadedSurface  = IMG_Load( path );
  if( loadedSurface == NULL ) {
    printf( "Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError() );
  }
  else {
    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
    if( newTexture == NULL ) {
      printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
    }
    SDL_FreeSurface( loadedSurface );
  }

  return newTexture;
}

bool init() {
  bool success = true;

  if( SDL_Init(SDL_INIT_VIDEO ) < 0) {
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    success = false;
  }
else {
  gWindow = SDL_CreateWindow( "Bad Bo Gay-me", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
  SDL_SetWindowResizable( gWindow, true );
  if( gWindow == NULL ) {
    printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
    success = false;
  }
  else {
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
    if( gRenderer == NULL ) {
      printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
      success = false;
    }
    else {
      SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

      int imgFlags = IMG_INIT_PNG;
      if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        success = false;
      }
      else {
        gScreenSurface = SDL_GetWindowSurface( gWindow );
      }
    }
  }
}

return success;
}

bool loadMedia() {
  bool success = true;

  gTexture = loadTexture( "player.png" );
  if( gTexture == NULL ) {
    printf( "Failed to load texture image!\n" );
    success = false;
  }
  gFloor = loadTexture( "floor.png" );
  if( gFloor == NULL ) {
    printf( "Failed to load floor image!/n" );
    success = false;
  }
  gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadTexture( "player.png" );
  if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL ) {
    printf( "Failed to load right image!\n ");
    success = false;
  }
  gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadTexture( "player_flip.png" );
  if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL ) {
    printf( "Failed to load left image!\n ");
    success = false;
  }
  return success;

}

void cleanup() {

  SDL_DestroyTexture( gTexture );
  SDL_DestroyTexture( gFloor );
  gTexture = NULL;
  gFloor = NULL;

  SDL_DestroyRenderer( gRenderer );
  SDL_DestroyWindow( gWindow );
  gRenderer = NULL;
  gWindow = NULL;

  IMG_Quit();
  SDL_Quit();
}

int main( int argc, char* args[] ) {
  Uint32 delta;

  bool success = true;

  if( !init() ) {
    printf( "Failed to initialize!\n");
  }
  else {
    if( !loadMedia() ) {
      printf( "Failed to load media!\n" );
    }
    else {
      bool quit = false;

      // SDL_Rect playerRect = { .x = 0, .y = 100, .w = 64 * 3, .h = 64 * 3 };
      // SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
      // SDL_RenderFillRect( gRenderer, &playerRect );

      struct Player* player = create_player(gRenderer);

      SDL_Rect floorRect = { .x = 0, .y = 710, .w = 1280, .h = 10 };
      SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
      SDL_RenderFillRect( gRenderer, &floorRect );

      Uint32 time_of_last_frame = SDL_GetTicks();

      SDL_Event e;
      while( !quit ) {

        Uint32 frame_delta = SDL_GetTicks() - time_of_last_frame;
        time_of_last_frame = SDL_GetTicks();

        while( SDL_PollEvent( &e ) != 0 ) {
          if( e.type == SDL_QUIT ) {
            quit = true;
          }

          move_player(player);
        }
        SDL_SetRenderDrawColor( gRenderer, 232, 59, 192, 1 );
        SDL_RenderClear( gRenderer );

        SDL_Rect pRect = { .x = player->posX, .y = player->posY, .w = player->width, .h = player->height};
        SDL_RenderCopy( gRenderer, gTexture, NULL, &pRect );
        SDL_RenderCopy( gRenderer, gFloor, NULL, &floorRect );

        SDL_RenderPresent( gRenderer );
      }
    }
  }
  cleanup();

  return 0;
}
