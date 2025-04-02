#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <SDL_mixer.h>
#include "click_mouse.h"
#include "score.h"

bool init(SDL_Window* &, SDL_Renderer* &);
bool initaudio (Mix_Music* &, Mix_Chunk* &);
bool showMenu (SDL_Renderer* &);
bool loadTextureToRenderer(SDL_Renderer *, SDL_Texture* &, std::string );
SDL_Texture* loadTextureFromSurface(std::string , SDL_Renderer *);
void close(SDL_Window *, SDL_Renderer *, SDL_Texture *);

#endif
