#ifndef SCORE_H
#define SCORE_H


#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

struct display
{
    int x;
    std::string xToString;
    SDL_Surface *surface;
    SDL_Texture *texture;

    display (int x_ = 0, SDL_Surface *surface_ = nullptr, SDL_Texture *texture_ = nullptr)
    {
        x = x_;
        surface = surface_;
        texture = texture_;
    }

    display (int x_,std::string xToString_, SDL_Surface *surface_, SDL_Texture *texture_)
    {
        x = x_;
        xToString = xToString_;
        surface = surface_;
        texture = texture_;
    }
};

void transmit (TTF_Font* &);
void draw (const int&, std::string &, SDL_Color&, SDL_Surface* &, SDL_Texture* &, SDL_Renderer* &, int);

#endif