#include "score.h"
#include <iostream>

using namespace std;

TTF_Font *font = nullptr;

void transmit (TTF_Font* &inMain)
{
    font = inMain;
}


void drawTexture (const int &x, string &xToString, SDL_Color &color ,SDL_Surface* &surface, SDL_Texture* &texture, SDL_Renderer* &renderer, int check)
{
    if (check == 0)
    {
        xToString = "SCORE : " + to_string (x);
    }
    else if (check == 1)
    {
        xToString = "HIGHSCORE : " + to_string (x);
    }
    else if (check == 2)
    {
        xToString = "TIME : " + to_string (x);
    }
    else xToString = "" + to_string (x);
    
    surface = TTF_RenderText_Blended (font, xToString.c_str (), color);
    if (!surface)
    {
        cout << "ttf_rendertext_blended " << TTF_GetError () << endl;
    }
    else
    {
        if (texture)
        {
            SDL_DestroyTexture (texture);
        }
        texture = SDL_CreateTextureFromSurface (renderer, surface);
        if (!texture)
        {
            cout << "text score " << SDL_GetError () << endl; 
            SDL_FreeSurface (surface);
        }
    }
    SDL_FreeSurface (surface);
    surface = nullptr;
}
