#ifndef FS_MODEL
#define FS_MODEL

#include <vector>
#include "mesh.hpp"
#include "texture.hpp"

namespace Firesteel {
    struct Model {
        std::vector<Texture> textures;
        std::vector<Mesh> meshes;
        std::string path;
        std::string getDirectory() const {
            return path.substr(0, path.find_last_of('\\'));
        }
        Model(const std::string& tPath = "") {
            textures.clear();
            meshes.clear();
            path = tPath;
        }
    };
}

#endif // !FS_MODEL