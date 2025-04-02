#include <SDL.h>
#include <SDL_image.h>
#include "click_mouse.h"

using namespace std;

bool isInsideRect (SDL_Rect &rect, int x, int y)
{
    return (x >= rect.x && x <=rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h );
}

int side (const int &temp, int &index, const int &holes)
{
    while (temp == index)
    {
        index = rand () % holes;
    }
    return index;
}

bool check_key (SDL_Event &e, int &index)
{
    SDL_Keycode key [] = {SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6};
    for (int i=1; i<=6; i++)
    {
        if (e.key.keysym.sym == key [index]) return true;
    }
    return false;
}
