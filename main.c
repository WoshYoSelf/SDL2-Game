#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

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
  gTexture = NULL;

  SDL_DestroyRenderer( gRenderer );
  SDL_DestroyWindow( gWindow );
  gRenderer = NULL;
  gWindow = NULL;

  IMG_Quit();
  SDL_Quit();
}

int main( int argc, char* args[] ) {
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

      SDL_Event e;
      SDL_Rect fillRect = { .x = 10, .y = 50, .w = 64 * 3, .h = 64 * 3 };
      SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
      SDL_RenderFillRect( gRenderer, &fillRect );

      while( !quit ) {
        while( SDL_PollEvent( &e ) != 0 ) {
          if( e.type == SDL_QUIT ) {
            quit = true;
          }
          if( SDL_GetKeyboardState( NULL )[ SDL_SCANCODE_Q ] ) {
            quit = true;
          }
          else if( e.type == SDL_KEYDOWN ) {
            switch( e.key.keysym.sym ) {
              case SDLK_d:
                gTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                fillRect.x += 10;
                break;

              case SDLK_a:
                gTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                fillRect.x -= 10;
                break;
            }
            switch( e.key.keysym.sym ) {
              case SDLK_d:
                gTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                break;
            }
          }
        }

        SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 255 );
        SDL_RenderClear( gRenderer );

        SDL_RenderCopy( gRenderer, gTexture, NULL, &fillRect );

        SDL_RenderPresent( gRenderer );
      }
    }
  }
  cleanup();

  return 0;
}
