#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "color.h"
#include "framebuffer.h"
#include "objloader.h"
#include "triangle.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Color currentColor;

std::vector<glm::vec3> vertices;
std::vector<Face> faces;

// Rotacion para ver la nave en vista 3/4 desde arriba
glm::mat4 rotation(1.0f);
// Centro (ya rotado) del modelo y escala para que quepa en pantalla
glm::vec3 modelCenter(0.0f);
float modelScale = 1.0f;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Software Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void setColor(const Color& color) {
    currentColor = color;
}

glm::vec3 rotate(const glm::vec3& v) {
    return glm::vec3(rotation * glm::vec4(v, 1.0f));
}

// Calcula centro y escala a partir de la caja que envuelve al modelo rotado,
// para que quede centrado y ocupe ~70% de la pantalla
void fitModelToScreen() {
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f))
             * glm::rotate(glm::mat4(1.0f), glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 minV(std::numeric_limits<float>::max());
    glm::vec3 maxV(std::numeric_limits<float>::lowest());
    for (const glm::vec3& v : vertices) {
        glm::vec3 r = rotate(v);
        minV = glm::min(minV, r);
        maxV = glm::max(maxV, r);
    }

    modelCenter = (minV + maxV) * 0.5f;
    glm::vec3 size = maxV - minV;
    modelScale = 0.7f * std::min(SCREEN_WIDTH / size.x, SCREEN_HEIGHT / size.y);
}

// Lleva un vertice del modelo a coordenadas de pantalla
glm::vec3 toScreen(const glm::vec3& v) {
    glm::vec3 r = (rotate(v) - modelCenter) * modelScale;
    // En pantalla y crece hacia abajo, por eso se invierte
    return glm::vec3(SCREEN_WIDTH / 2.0f + r.x, SCREEN_HEIGHT / 2.0f - r.y, r.z);
}

void render() {
    // Recorremos cada cara y usamos sus indices para obtener los vertices
    for (const Face& face : faces) {
        const std::vector<int>& idx = face.vertexIndices;

        // Las caras de la nave son cuadrilateros: se dividen en triangulos
        // en abanico (0,1,2), (0,2,3), ...
        for (size_t i = 1; i + 1 < idx.size(); i++) {
            glm::vec3 A = toScreen(vertices[idx[0]]);
            glm::vec3 B = toScreen(vertices[idx[i]]);
            glm::vec3 C = toScreen(vertices[idx[i + 1]]);
            triangle(A, B, C);
        }
    }
}

int main(int argc, char* argv[]) {
    std::string modelPath = "models/nave.obj";
    const char* screenshotPath = nullptr;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--screenshot" && i + 1 < argc) {
            screenshotPath = argv[++i];
        } else {
            modelPath = arg;
        }
    }

    if (!loadOBJ(modelPath, vertices, faces)) {
        return 1;
    }
    std::cout << "Vertices: " << vertices.size() << ", caras: " << faces.size() << std::endl;
    fitModelToScreen();

    init();
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s) {
                saveBuffer("screenshot.bmp");
                std::cout << "Captura guardada en screenshot.bmp" << std::endl;
            }
        }

        clear();

        setColor(Color(255, 255, 0));
        render();

        renderBuffer(renderer);

        // Modo captura: guarda un frame y termina
        if (screenshotPath) {
            saveBuffer(screenshotPath);
            running = false;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
