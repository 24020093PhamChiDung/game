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

enum wordToNum { SCORE, HIGHSCORE, TIME };

const int holes = 6;
const Uint32 TimeRandom = 1200;
const int Length = 80;
const int Width = 95;
const int randomRatio = 100;
const int timePlay = 5;
const int FPS = 60;
const float FRAME_TIME = 1000.0f / FPS;

const SDL_Point hole[holes] = {{120, 250}, {415, 282}, {650, 364}, {258, 383}, {60, 486}, {497, 497}};

int main() {
    srand(time(0));
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Mix_Music* music = nullptr;
    Mix_Chunk* voice = nullptr;
    SDL_Texture* background = nullptr;
    SDL_Texture* mouse = nullptr;
    SDL_Texture* diamond = nullptr;
    SDL_Texture* bom = nullptr;
    SDL_Texture* bomExplode = nullptr;
    display score;
    display highScore;
    display time;
    bool restart = false;

    SDL_Color color = {0, 0, 0, 255};
    score.rect = {670, 0, 100, 70};
    highScore.rect = {600, 70, 180, 70};
    time.rect = {355, 0, 90, 50};

    if (TTF_Init() == -1) {
        cout << "ttf init " << TTF_GetError() << endl;
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("Comic Sans MS Bold.ttf", 50);
    if (!font) {
        cout << "font " << TTF_GetError() << endl;
        TTF_Quit();
        return -1;
    }

    if (!init(window, renderer) || !initaudio(music, voice) ||
        !loadTextureToRenderer(renderer, background, "image/background.png") ||
        !loadTextureToRenderer(renderer, mouse, "image/mouse.png") ||
        !loadTextureToRenderer(renderer, diamond, "image/kimcuong.png") ||
        !loadTextureToRenderer(renderer, bom, "image/bom.png") ||
        !loadTextureToRenderer(renderer, bomExplode, "image/bomExplode.png")) {
        cout << "Initialization failed" << endl;
        TTF_CloseFont(font);
        TTF_Quit();
        return -1;
    }

    if (!showMenu (renderer))
    {
            Mix_FreeMusic (music);
            Mix_FreeChunk (voice);
            SDL_DestroyTexture (bom);
            SDL_DestroyTexture (diamond);
            SDL_DestroyTexture (mouse);
            SDL_DestroyTexture (bomExplode);
            Mix_CloseAudio ();
            TTF_CloseFont(font);
            TTF_Quit ();
            Mix_Quit ();
            close (window, renderer, background);
            return 0;
    }

    do {
        transmit(font);
        
        Mix_VolumeMusic(32);
        Mix_PlayMusic(music, -1);

        ifstream file("highScore.txt");
        if (file.is_open()) {
            file >> highScore.x;
            file.close();
        } else {
            highScore.x = 0;
        }
        int HIGHSCORE = highScore.x;

        // Reset trạng thái game
        score.x = 0;
        time.x = timePlay;
        SDL_Event e;
        bool run = true;
        int index = rand() % holes;
        int temp = index;
        int randomImage = (rand() % randomRatio) + 1;
        int count = 0;
        float deltaTime = 0;
        Uint32 lastTime = SDL_GetTicks();
        Uint32 timeStart = SDL_GetTicks();
        Uint32 Time = SDL_GetTicks();
        Uint32 timeCurrent = SDL_GetTicks();

        drawTexture(score.x, score.xToString, color, score.surface, score.texture, renderer, SCORE);
        drawTexture(highScore.x, highScore.xToString, color, highScore.surface, highScore.texture, renderer, wordToNum::HIGHSCORE);
        drawTexture(time.x, time.xToString, color, time.surface, time.texture, renderer, TIME);

        while (run) {
            if (count == timePlay) break;
            Uint32 frameStart = SDL_GetTicks();

            SDL_Rect frame_hole = {hole[index].x, hole[index].y, Width, Length};
            SDL_Rect displayRect = frame_hole;

            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT || SDL_GetTicks() - timeStart >= timePlay * 1000) {
                    run = false;
                } else if ((e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) || (e.type == SDL_KEYDOWN)) {
                    Mix_PlayChannel(-1, voice, 0);
                    if (isInsideRect(frame_hole, e.button.x, e.button.y) || check_key(e, index)) {
                        if (randomImage <= 80) score.x += 5;
                        else if (randomImage > 80 && randomImage <= 90) score.x += 10;
                        else {
                            SDL_RenderClear(renderer);
                            SDL_RenderCopy(renderer, background, nullptr, nullptr);
                            SDL_RenderCopy(renderer, score.texture, nullptr, &score.rect);
                            SDL_RenderCopy(renderer, highScore.texture, nullptr, &highScore.rect);
                            SDL_RenderCopy(renderer, time.texture, nullptr, &time.rect);
                            SDL_RenderCopy(renderer, bomExplode, nullptr, &frame_hole);
                            SDL_RenderPresent(renderer);
                            Uint32 timeNow = SDL_GetTicks();
                            while (SDL_GetTicks() - timeNow < 300) {}
                            score.x -= 10;
                        }

                        if (score.x >= highScore.x) {
                            highScore.x = score.x;
                            drawTexture(score.x, score.xToString, color, score.surface, score.texture, renderer, SCORE);
                            drawTexture(highScore.x, highScore.xToString, color, highScore.surface, highScore.texture, renderer, wordToNum::HIGHSCORE);
                        } else {
                            drawTexture(score.x, score.xToString, color, score.surface, score.texture, renderer, SCORE);
                        }

                        temp = side(temp, index, holes);
                        randomImage = (rand() % randomRatio) + 1;
                        Time = SDL_GetTicks();
                    }
                }
            }

            if (SDL_GetTicks() - timeCurrent > 1000) {
                count++;
                timeCurrent = SDL_GetTicks();
                time.x--;
                drawTexture(time.x, time.xToString, color, time.surface, time.texture, renderer, TIME);
            }

            if (SDL_GetTicks() - Time >= TimeRandom) {
                temp = side(temp, index, holes);
                index = temp;
                randomImage = (rand() % randomRatio) + 1;
                Time = SDL_GetTicks();
            }

            deltaTime = (SDL_GetTicks() - lastTime) / 1000.0f;
            lastTime = SDL_GetTicks();

            animation(displayRect, deltaTime, hole[index]);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background, nullptr, nullptr);
            SDL_RenderCopy(renderer, score.texture, nullptr, &score.rect);
            SDL_RenderCopy(renderer, highScore.texture, nullptr, &highScore.rect);
            SDL_RenderCopy(renderer, time.texture, nullptr, &time.rect);
            if (randomImage <= 80) SDL_RenderCopy(renderer, mouse, nullptr, &displayRect);
            else if (randomImage > 80 && randomImage <= 90) SDL_RenderCopy(renderer, diamond, nullptr, &displayRect);
            else SDL_RenderCopy(renderer, bom, nullptr, &displayRect);
            SDL_RenderPresent(renderer);

            Uint32 frameDuration = SDL_GetTicks() - frameStart;
            if (frameDuration < FRAME_TIME) {
                SDL_Delay(FRAME_TIME - frameDuration);
            }
        }

        if (score.x > HIGHSCORE) {
            ofstream file("highScore.txt", ofstream::trunc);
            if (file.is_open()) {
                file << score.x;
                file.close();
            }
        }

        Mix_VolumeMusic(0);
        Mix_PauseMusic(); // Tạm dừng nhạc để tránh chồng lấn khi restart
        if (lastMenu(renderer, score, time, highScore, background)) {
            restart = true;
            SDL_DestroyTexture(score.texture);
            SDL_DestroyTexture(highScore.texture);
            SDL_DestroyTexture(time.texture);
            continue;
        } else {
            restart = false;
        }
    } while (restart);

    // Giải phóng toàn bộ tài nguyên khi thoát
    Mix_FreeMusic(music);
    Mix_FreeChunk(voice);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(mouse);
    SDL_DestroyTexture(diamond);
    SDL_DestroyTexture(bom);
    SDL_DestroyTexture(bomExplode);
    SDL_DestroyTexture(score.texture);
    SDL_DestroyTexture(highScore.texture);
    SDL_DestroyTexture(time.texture);
    Mix_CloseAudio();
    TTF_CloseFont(font);
    TTF_Quit();
    Mix_Quit();
    close(window, renderer, background);
    return 0;
}