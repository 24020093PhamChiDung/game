#ifndef SCORE_H
#define SCORE_H


#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

void transmit (TTF_Font* &);
void drawScore (const int&, std::string &, SDL_Color&, SDL_Surface* &, SDL_Texture* &, SDL_Renderer* &);

#endif