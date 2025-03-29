#include "score.h"
#include <iostream>

using namespace std;

TTF_Font *font = nullptr;

void transmit (TTF_Font* &inMain)
{
    font = inMain;
}


void drawScore (const int &score, string &string_score, SDL_Color &color ,SDL_Surface* &image_score, SDL_Texture* &Score, SDL_Renderer* &renderer)
{
    string_score = "SCORE : " + to_string (score);
    image_score = TTF_RenderText_Blended (font, string_score.c_str (), color);
    if (!image_score)
    {
        cout << "ttf_rendertext_solid " << TTF_GetError () << endl;
    }
    else
    {
        Score = SDL_CreateTextureFromSurface (renderer, image_score);
        if (!Score)
        {
            cout << "text score " << SDL_GetError () << endl; 
            SDL_FreeSurface (image_score);
        }
    }
}
