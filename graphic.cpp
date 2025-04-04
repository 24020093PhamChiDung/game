#include "graphic.h"
#include <iostream>

using namespace std;

const string title = "WHACK A MOLE";
const int width = 800;
const int height = 600;

SDL_Color color = {0, 0, 0, 255};

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





bool showMenu(SDL_Renderer* &renderer )
{
    bool inMenu = true;
    SDL_Event e;

    SDL_Texture* menu = nullptr;
    //SDL_Texture* start = nullptr;
    // 295, 540, 230, 30
    SDL_Rect play = {295, 540, 230, 30};

    loadTextureToRenderer(renderer, menu, "image/menu.jpg");
    while (inMenu)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
               
                SDL_DestroyTexture (menu);
                return false;
            }

            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                if (isInsideRect (play, e.button.x, e.button.y))
                {
                    SDL_DestroyTexture (menu);
                    return true;
                }
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, menu, nullptr, nullptr);
        SDL_RenderPresent(renderer);

    }
    return false;
}



bool lastMenu (SDL_Renderer* &renderer, display &score, display &time, display &highScore, SDL_Texture* &background)
{
    SDL_Event e;
    bool run = true;
    bool check = false;
    SDL_Texture *lastmenu = nullptr; 
    SDL_Texture *restart = nullptr;

    loadTextureToRenderer (renderer, lastmenu, "image/yourScore.png");
    loadTextureToRenderer (renderer, restart, "image/restart.png");

    SDL_Rect menu = {200, 200, 400, 200};
    SDL_Rect Score = {300, 250, 200, 100};
    SDL_Rect Restart = {310, 400, 180, 100};

    display scoreInlastMenu;
    scoreInlastMenu.x = score.x;
    drawTexture (scoreInlastMenu.x, scoreInlastMenu.xToString, color, scoreInlastMenu.surface, scoreInlastMenu.texture, renderer, 10);


    while (run)
    {
        while (SDL_PollEvent (&e))
        {
            if (e.type == SDL_QUIT)
            {
                run = false;
                SDL_DestroyTexture (lastmenu);
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                if (isInsideRect (Restart, e.button.x, e.button.y))
                {
                    check = true;
                    run = false;
                    break;
                }
            }
        }

        SDL_RenderClear (renderer);
        SDL_RenderCopy(renderer, background, nullptr, nullptr);  // Vẽ background
        SDL_RenderCopy (renderer, score.texture, nullptr, &score.rect);  // Vẽ score
        SDL_RenderCopy (renderer, highScore.texture, nullptr, &highScore.rect);
        SDL_RenderCopy (renderer, time.texture, nullptr, &time.rect);
        SDL_RenderCopy (renderer, lastmenu, nullptr, &menu);
        SDL_RenderCopy (renderer, restart, nullptr, &Restart);
        SDL_RenderCopy (renderer, scoreInlastMenu.texture, nullptr, &Score);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture (lastmenu);
    SDL_DestroyTexture (restart);
    SDL_DestroyTexture (scoreInlastMenu.texture);
    return check;
}



bool initaudio (Mix_Music* &music, Mix_Chunk* & voice)
{
    if (Mix_OpenAudio (44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        return false;
    }
    music = Mix_LoadMUS ("audio/music.mp3");   // trả về mix_music trên heap
    if (! music)
    {
        cout << "background music " << Mix_GetError () << endl;
        return false;
    }

    voice = Mix_LoadWAV ("audio/pounding.wav"); // trả về mix_chunk trên heap
    if (! voice)
    {
        Mix_FreeMusic (music);
        cout << "load voice " << Mix_GetError () << endl;
        return false;
    }
    Mix_VolumeMusic (32);
    Mix_VolumeChunk (voice, 128);
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


void animation (SDL_Rect &displayRect, float &deltaTime, const SDL_Point &basePos) 
{
    static float time = 0;
    static Uint32 lastMoveTime = 0;

    time += deltaTime;
    float offsetX = sin(time * 3) * 3; // Lắc lư theo sin

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastMoveTime > 2000) { // 2000 ms = 2 giây
        lastMoveTime = currentTime;
        displayRect.x += (rand() % 11) - 5; // Dịch ngẫu nhiên -5 đến +5 px
        displayRect.y += (rand() % 11) - 5;
    }

    // Giữ vị trí hiển thị dựa trên vị trí cơ bản + hiệu ứng
    displayRect.x = basePos.x + offsetX * deltaTime * 60; // Tốc độ mượt mà hơn
    displayRect.y = basePos.y; // Giữ y cố định hoặc thêm hiệu ứng nếu muốn
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


