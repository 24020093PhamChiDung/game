#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include "graphic.h"
#include "click_mouse.h"

using namespace std;

const int holes = 6;

const SDL_Point hole[holes] = {{120, 250}, {423, 282}, {650,364}, {266, 383}, {60, 486}, {491, 497}};


void updateMouse(SDL_Rect &mouseRect, float deltaTime) {
    static float time = 0;
    static Uint32 lastMoveTime = 0;

    // Lắc lư nhẹ theo sin
    time += deltaTime;
    mouseRect.x += sin(time * 3) * 3; // Biên độ 3px lắc lư qua lại

    // Cứ mỗi 2 giây, di chuyển ngẫu nhiên 1 chút
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastMoveTime > 2000) { // 2000 ms = 2 giây
        lastMoveTime = currentTime;
        mouseRect.x += (rand() % 11) - 5; // Dịch ngẫu nhiên -5 đến +5 px
        mouseRect.y += (rand() % 11) - 5;
    }
}


int main ()
{
    srand (time (0));
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *background = nullptr;
    SDL_Texture *mouse = nullptr;
    SDL_Texture *Score = nullptr;
    SDL_Surface *image_score = nullptr;
    SDL_Color color = {0, 0, 0, 255};
    SDL_Rect frame_score = {670, 0, 100, 70};

    if (TTF_Init() == -1)
    {
        cout << "ttf init " << TTF_GetError() << endl;
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("Apple Chancery.ttf", 24); // Tải font, cỡ chữ 24
    if (!font) 
    {
        cout << "font  " << TTF_GetError() << endl;
        return -1;
    }
    int score = 0;
    
    if (init (window, renderer) && loadTextureToRenderer (renderer, background,"image/background.png") && loadTextureToRenderer (renderer, mouse, "image/mouse.png"))
    {
        SDL_Event e;
        bool run = true;
        SDL_Rect frame;
        int index = rand () % holes;
        int temp = index;
        double deltaTime = 0;
        double lastTime = SDL_GetTicks ();
        Uint32 Time = SDL_GetTicks ();
        
        while (run)
        {
            frame = {hole[index].x, hole[index].y, 85, 85};
            while (SDL_PollEvent(&e) != 0)
            {       
                if (e.type == SDL_QUIT) run = false;
                else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                {
                    if (isInsideRect (frame, e.button.x, e.button.y))
                    {
                        if (image_score)
                        {
                            SDL_FreeSurface (image_score);
                            SDL_DestroyTexture (Score);
                        }
                        score += 5;
                        string string_score = "SCORE : " + to_string (score);
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
                        if (isInsideRect (frame, e.button.x, e.button.y))
                        {
                            while (index == temp)
                            {   
                                index = rand () % holes;
                            }
                            temp = index;
                        }
                        Time = SDL_GetTicks ();
                    }
                }   
            }
            if (SDL_GetTicks () - Time >= 1200)
            {
                while (index == temp)
                {
                    index = rand () % holes;
                }
                temp = index;
                Time = SDL_GetTicks ();
            }
            
            deltaTime = (SDL_GetTicks() - lastTime) / 1000.0f;
            lastTime = SDL_GetTicks ();

            updateMouse(frame, deltaTime); // Cập nhật hiệu ứng di chuyển
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background, nullptr, nullptr);  // Vẽ background
            SDL_RenderCopy (renderer, Score, nullptr, &frame_score);  // Vẽ score
            SDL_RenderCopy(renderer, mouse, nullptr, &frame);   // Vẽ chuột
            SDL_RenderPresent(renderer);
        }
        SDL_FreeSurface (image_score);
        SDL_DestroyTexture (mouse);
        SDL_DestroyTexture (Score);
        TTF_CloseFont(font);
        close (window, renderer, background);
    }
    return 0;
}