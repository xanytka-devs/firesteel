#ifndef FS_MODEL_H
#define FS_MODEL_H

#include "mesh.hpp"

namespace Firesteel {
    struct DrawData {
        std::vector<Texture> textures;
        void remove() {
            for (size_t i=0; i < textures.size(); i++)
                textures[i].remove();
        }
    };
    struct Model {
        std::string path;
        std::vector<DrawData> materials;
        std::vector<Mesh> meshes;
        std::string getDirectory() const {
            return path.substr(0, path.find_last_of('\\'));
        }
        Model(const std::string& tPath="") {
            materials.clear();
            meshes.clear();
            path=tPath;
        }
    };
}

#endif // !FS_MODEL_H