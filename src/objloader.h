#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

// Una cara del OBJ: guarda los indices (base 0) de sus vertices
struct Face {
    std::vector<int> vertexIndices;
};

// Lee las lineas "v" y "f" de un archivo .obj
bool loadOBJ(const std::string& path, std::vector<glm::vec3>& vertices, std::vector<Face>& faces);
