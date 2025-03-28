#include "graphic.h"
#include <iostream>

using namespace std;

const string title = "WHACK A MOLE";
const int width = 800;
const int height = 600;

// Tạo cửa sổ và renderer
bool init(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        cout << "Init SDL failed: " << SDL_GetError() << endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        cout << "IMG_Init failed: " << IMG_GetError() << endl;
        return false;
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
        cout << "Warning: Linear texture filtering not enabled!" << endl;
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window)
    {
        cout << "Create window failed: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        cout << "Create renderer failed: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        return false;
    }
    return true;
}

// Tải ảnh lên 
bool loadTextureToRenderer(SDL_Renderer *renderer, SDL_Texture* &image, string path)
{
    image = loadTextureFromSurface(path, renderer);
    if (!image)
    {
        cout << "Load media failed: " << IMG_GetError() << endl;
        return false;
    }
    return true;
}

// Tải ảnh thông qua surface
SDL_Texture* loadTextureFromSurface(string path, SDL_Renderer *renderer)
{
    SDL_Texture *image = nullptr;
    SDL_Surface *loadsurface = IMG_Load(path.c_str());
    if (!loadsurface)
    {
        cout << "Load surface failed: " << IMG_GetError() << endl;
        return nullptr;
    }

    image = SDL_CreateTextureFromSurface(renderer, loadsurface);   // trả về texture trên heap
    if (!image)
    {
        cout << "Create texture failed : " << SDL_GetError() << endl;
        SDL_FreeSurface(loadsurface);
        return nullptr;
    }
    SDL_FreeSurface(loadsurface);
    return image;
}

// Đóng chương trình
void close(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *image)
{
    if (image) SDL_DestroyTexture(image);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    IMG_Quit(); 
    SDL_Quit();
}


