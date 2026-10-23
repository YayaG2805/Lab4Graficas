#include "objloader.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool loadOBJ(const std::string& path, std::vector<glm::vec3>& vertices, std::vector<Face>& faces) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir " << path << std::endl;
        return false;
    }

    std::string lineText;
    while (std::getline(file, lineText)) {
        std::istringstream iss(lineText);
        std::string type;
        iss >> type;

        if (type == "v") {
            glm::vec3 v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (type == "f") {
            Face face;
            std::string token;
            // Cada token tiene la forma v, v/vt, v//vn o v/vt/vn; solo nos interesa v
            while (iss >> token) {
                int index = std::stoi(token.substr(0, token.find('/')));
                // Los indices del OBJ empiezan en 1; los negativos cuentan desde el final
                face.vertexIndices.push_back(index > 0 ? index - 1 : static_cast<int>(vertices.size()) + index);
            }
            faces.push_back(face);
        }
    }

    return true;
}
