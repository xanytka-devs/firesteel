#ifndef MATERIAL_H
#define MATERIAL_H

#include "../math.hpp"
#include "shader.hpp"
#include "../utils.hpp"
#include <fstream>
#include "../json.hpp"
using json = nlohmann::json;

namespace firesteel {
    /// <summary>
    /// Basic class for shader storage & loading.
    /// </summary>
    struct Material {
        Shader shader;
        std::string name = "New Material";
        bool usable = true;

        /// <summary>
        /// Destroy shader (cleanup).
        /// </summary>
        void remove() { shader.remove(); }
        /// <summary>
        /// Load material properties from file.
        /// </summary>
        /// <param name="t_mat_file">File path.</param>
        void load(const char* t_mat_file) {
            if(!std::filesystem::exists(t_mat_file)) { usable = false; return; }
            std::ifstream f(t_mat_file);
            json data = json::parse(f);
            std::string vert = data.at("shaders").value("vertex", "");
            std::string frag = data.at("shaders").value("fragment", "");
            std::string geom = data.at("shaders").value("geometry", "");
            if(geom != "") shader = Shader(vert.c_str(), frag.c_str(), geom.c_str());
            else shader = Shader(vert.c_str(), frag.c_str());
            name = data.value("name", "New Material");
            if(data.contains("variables")) {
                shader.enable();
                for(auto& i : data.at("variables").items()) {
                    if(data.at("variables").at(i.key()).is_array()) {
                        if(data.at("variables").at(i.key()).size() == 1)
                            shader.set_4_floats("material." + i.key(),
                                jskey(data, i.key(), 0),
                                jskey(data, i.key(), 0),
                                jskey(data, i.key(), 0),
                                jskey(data, i.key(), 0));
                        else if(data.at("variables").at(i.key()).size() == 2)
                            shader.set_2_floats("material." + i.key(),
                                jskey(data, i.key(), 0),
                                jskey(data, i.key(), 1));
                        else if (data.at("variables").at(i.key()).size() == 3)
                            shader.set_3_floats("material." + i.key(),
                                jskey(data, i.key(), 0),
                                jskey(data, i.key(), 1),
                                jskey(data, i.key(), 2));
                        else if (data.at("variables").at(i.key()).size() == 4)
                            shader.set_4_floats("material." + i.key(),
                                jskey(data, i.key(), 0),
                                jskey(data, i.key(), 1),
                                jskey(data, i.key(), 2),
                                jskey(data, i.key(), 3));
                    } else {
                        if(data.at("variables").at(i.key()).is_number_float())
                            shader.set_float("material." + i.key(), data.at("variables").value(i.key(), 0.0f));
                        else if(data.at("variables").at(i.key()).is_number_integer()) 
                            shader.set_int("material." + i.key(), data.at("variables").value(i.key(), 0));
                    }
                }
                shader.disable();
            }
        }
    private:
        float jskey(json t_data, const std::string t_key, size_t t_id) {
            return t_data.at("variables").at(t_key)[t_id];
        }
    };
}

#endif // MATERIAL_H