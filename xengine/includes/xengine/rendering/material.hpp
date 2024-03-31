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
        const char* name = "New Material";

        void load(const char* t_mat_file) {
            std::ifstream f(t_mat_file);
            json data = json::parse(f);
            shader = Shader(data.at("shaders").value("vertex", "").c_str(),
                data.at("shaders").value("fragment", "").c_str());
            //Compatability.
            if(data.contains("variables")) {
                shader.enable();
                for(auto& i : data.at("variables").items()) {
                    if(data.at("variables").at(i.key()).is_array()) {
                        if(data.at("variables").at(i.key()).size() == 1)
                            shader.set_4_floats(i.key(),
                                data.at("variables").at(i.key())[0],
                                data.at("variables").at(i.key())[0],
                                data.at("variables").at(i.key())[0],
                                data.at("variables").at(i.key())[0]);
                        else if (data.at("variables").at(i.key()).size() == 2)
                            shader.set_2_floats(i.key(),
                                data.at("variables").at(i.key())[0],
                                data.at("variables").at(i.key())[1]);
                        else if(data.at("variables").at(i.key()).size() == 3)
                            shader.set_3_floats(i.key(),
                                data.at("variables").at(i.key())[0],
                                data.at("variables").at(i.key())[1],
                                data.at("variables").at(i.key())[2]);
                        else if(data.at("variables").at(i.key()).size() == 4)
                            shader.set_4_floats(i.key(),
                                data.at("variables").at(i.key())[0],
                                data.at("variables").at(i.key())[1],
                                data.at("variables").at(i.key())[2],
                                data.at("variables").at(i.key())[3]);
                    } else {
                        if(data.at("variables").at(i.key()).is_number_float())
                            shader.set_float(i.key(), data.at("variables").value(i.key(), 0.0f));
                        else if(data.at("variables").at(i.key()).is_number_integer())
                            shader.set_int(i.key(), data.at("variables").value(i.key(), 0));
                    }
                }
                shader.disable();
            }
        }
    };
}

#endif // MATERIAL_H