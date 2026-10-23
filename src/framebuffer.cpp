#include "framebuffer.h"
#include <algorithm>

std::vector<Color> framebuffer(SCREEN_WIDTH * SCREEN_HEIGHT);
Color clearColor(0, 0, 0);

void clear() {
    std::fill(framebuffer.begin(), framebuffer.end(), clearColor);
}

void point(int x, int y) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
        return;
    }
    framebuffer[y * SCREEN_WIDTH + x] = currentColor;
}

// Convierte el framebuffer a una superficie RGBA de SDL
static SDL_Surface* bufferToSurface() {
    return SDL_CreateRGBSurfaceWithFormatFrom(
        framebuffer.data(), SCREEN_WIDTH, SCREEN_HEIGHT, 32,
        SCREEN_WIDTH * sizeof(Color), SDL_PIXELFORMAT_RGBA32);
}

void renderBuffer(SDL_Renderer* renderer) {
    SDL_Texture* texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_UpdateTexture(texture, nullptr, framebuffer.data(), SCREEN_WIDTH * sizeof(Color));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);
}

bool saveBuffer(const char* path) {
    SDL_Surface* surface = bufferToSurface();
    if (!surface) {
        return false;
    }
    bool ok = SDL_SaveBMP(surface, path) == 0;
    SDL_FreeSurface(surface);
    return ok;
}
