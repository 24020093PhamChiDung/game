#include <SDL.h>
#include <SDL_image.h>
#include "click_mouse.h"

using namespace std;

bool isInsideRect (SDL_Rect rect, int x, int y)
{
    return (x >= rect.x && x <=rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h );
}
