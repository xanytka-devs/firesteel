#ifndef MATERIAL_H
#define MATERIAL_H

#include "../math.hpp"
#include "../utils.hpp"
#include <fstream>
#include "../json.hpp"
using json = nlohmann::json;

namespace XEngine {
    struct Material {
        Shader shader;

        void load(const char* t_mat_file) {
            std::ifstream f(t_mat_file);
            json data = json::parse(f);
            shader = Shader(data.at("shaders").value("vertex", "").c_str(),
                data.at("shaders").value("fragment", "").c_str());
        }
    };
}

#endif // MATERIAL_H