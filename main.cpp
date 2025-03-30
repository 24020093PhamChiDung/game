#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "graphic.h"
#include "click_mouse.h"
#include "score.h"

using namespace std;

enum wordToNum {SCORE, HIGHSCORE, TIME};

const int holes = 6;
const Uint32 TimeRandom = 1200;
const int Length = 80;  // chiều dài hình chữ nhật
const int Width = 95;   // chiều rộng hình chũ nhật
const int randomRatio = 100;   // Phần trăm xuất hiện
const int timePlay = 20;

const SDL_Point hole[holes] = {{120, 250}, {415, 282}, {650,364}, {258, 383}, {60, 486}, {497, 497}};


void animation (SDL_Rect &mouseRect, float deltaTime) {
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
    Mix_Music *music = nullptr;
    Mix_Chunk *voice = nullptr;
    SDL_Texture *background = nullptr;
    SDL_Texture *mouse = nullptr;
    SDL_Texture *diamond = nullptr;
    SDL_Texture *bom = nullptr;
    display score;
    display highScore;
    display time = {timePlay};

    SDL_Color color = {0, 0, 0, 255};
    SDL_Rect frame_score = {670, 0, 100, 70};
    SDL_Rect frame_highScore = {600, 70, 180, 70};
    SDL_Rect frame_time = {355, 0, 90, 50};

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
    
    if (init (window, renderer) && initaudio (music, voice) && loadTextureToRenderer (renderer, background,"image/background.png") && loadTextureToRenderer (renderer, mouse, "image/mouse.png") && loadTextureToRenderer (renderer, diamond, "image/kimcuong.png") && loadTextureToRenderer (renderer, bom,"image/bom.png"))
    {
        ifstream file ("highScore.txt");
        file >> highScore.x;
        file.close ();
        int HIGHSCORE = highScore.x;
        SDL_Event e;
        bool run = true;
        SDL_Rect frame_hole;
        int index = rand () % holes;
        int temp = index;
        int randomImage = (rand () % randomRatio ) +1;
        //int score = 0; ///
        //string string_score;  ////
        float deltaTime = 0;
        Uint32 lastTime = SDL_GetTicks ();
        Uint32 Time = SDL_GetTicks ();
        Uint32 timeStart = SDL_GetTicks ();
        Uint32 timeCurrent = SDL_GetTicks ();
        Mix_VolumeChunk (voice, 128);
        transmit (font);
        draw (score.x, score.xToString, color, score.surface, score.texture, renderer, SCORE);
        draw (highScore.x, highScore.xToString, color, highScore.surface, highScore.texture, renderer, wordToNum::HIGHSCORE);
        draw (time.x, time.xToString, color, time.surface, time.texture, renderer, TIME);

        
        while (run)
        {
            frame_hole = {hole[index].x, hole[index].y, Width, Length};
            while (SDL_PollEvent(&e) != 0)
            {       
                if (SDL_GetTicks () - timeCurrent > 1000)
                {
                    timeCurrent = SDL_GetTicks ();
                    time.x --;
                    draw (time.x, time.xToString, color, time.surface, time.texture, renderer, TIME);
                }
                if (e.type == SDL_QUIT || SDL_GetTicks () - timeStart > timePlay * 1000)
                {
                    run = false;
                    SDL_FreeSurface (score.surface);
                    SDL_DestroyTexture (score.texture);
                }
                else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                {
                    Mix_PlayChannel (-1, voice, 0);
                    if (isInsideRect (frame_hole, e.button.x, e.button.y))
                    {
                        if (score.surface)
                        {
                            SDL_FreeSurface (score.surface);
                            SDL_DestroyTexture (score.texture);
                        }

                        if (randomImage <= 80) score.x +=5;
                        else if (randomImage > 80 && randomImage <=90) score.x +=10;
                        else score.x -= 10;

                        if (score.x >= highScore.x)
                        {
                            highScore.x = score.x;
                            draw (score.x, score.xToString, color, score.surface, score.texture, renderer, SCORE);
                            draw (highScore.x, highScore.xToString, color, highScore.surface, highScore.texture, renderer, wordToNum::HIGHSCORE);
                        }
                        else
                        {
                            draw (score.x, score.xToString, color, score.surface, score.texture, renderer, SCORE);
                        }

                        if (SDL_GetTicks () - timeCurrent > 1000)
                        {
                            timeCurrent = SDL_GetTicks ();
                            time.x --;
                            draw (time.x, time.xToString, color, time.surface, time.texture, renderer, TIME);
                        }
                        
                        temp = side (temp, index, holes);
                        randomImage = (rand () % randomRatio ) + 1;
                        Time = SDL_GetTicks ();
                    }
                }   
            }
            if (SDL_GetTicks () - Time >= TimeRandom)
            {
                temp = side (temp, index, holes);
                randomImage = (rand () % randomRatio) + 1;
                Time = SDL_GetTicks ();
            }
            
            deltaTime = (SDL_GetTicks() - lastTime) / 1000.0f;
            lastTime = SDL_GetTicks ();

            animation (frame_hole, deltaTime); // Cập nhật hiệu ứng di chuyển
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background, nullptr, nullptr);  // Vẽ background
            SDL_RenderCopy (renderer, score.texture, nullptr, &frame_score);  // Vẽ score
            SDL_RenderCopy (renderer, highScore.texture, nullptr, &frame_highScore);
            SDL_RenderCopy (renderer, time.texture, nullptr, &frame_time);
            if (randomImage <= 80) SDL_RenderCopy(renderer, mouse, nullptr, &frame_hole);   // Vẽ chuột
            else if (randomImage > 80 && randomImage <90) SDL_RenderCopy (renderer, diamond, nullptr, &frame_hole); // vẽ kim cương
            else SDL_RenderCopy (renderer, bom, nullptr, &frame_hole); // Vẽ bom
            SDL_RenderPresent(renderer);
        }

        if (score.x > HIGHSCORE)
        {
            ofstream file ("highScore.txt", ofstream::trunc);
            file << score.x;
            file.close ();
        }


        Mix_FreeMusic (music);
        Mix_FreeChunk (voice);
        SDL_FreeSurface (score.surface);
        SDL_FreeSurface (highScore.surface);
        SDL_FreeSurface (time.surface);
        SDL_DestroyTexture (bom);
        SDL_DestroyTexture (diamond);
        SDL_DestroyTexture (mouse);
        SDL_DestroyTexture (score.texture);
        SDL_DestroyTexture (highScore.texture);
        SDL_DestroyTexture (time.texture);
        Mix_CloseAudio ();
        TTF_CloseFont(font);
        TTF_Quit ();
        Mix_Quit ();
        close (window, renderer, background);
    }
    return 0;
}