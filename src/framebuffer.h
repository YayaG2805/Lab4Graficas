#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "color.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Color con el que se pintan los puntos (definido en main.cpp)
extern Color currentColor;

// Framebuffer: un Color por cada pixel de la pantalla
extern std::vector<Color> framebuffer;
extern Color clearColor;

// Llena todo el framebuffer con clearColor
void clear();

// Pinta un pixel con currentColor (ignora puntos fuera de la pantalla)
void point(int x, int y);

// Copia el framebuffer a la ventana de SDL
void renderBuffer(SDL_Renderer* renderer);

// Guarda el contenido del framebuffer como imagen BMP
bool saveBuffer(const char* path);
