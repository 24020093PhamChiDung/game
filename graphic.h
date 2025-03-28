#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

bool init(SDL_Window* &window, SDL_Renderer* &renderer);
bool loadTextureToRenderer(SDL_Renderer *renderer, SDL_Texture* &image, std::string path);
SDL_Texture* loadTextureFromSurface(std::string path, SDL_Renderer *renderer);
void close(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *image);

#endif
