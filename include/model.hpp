#ifndef FS_MODEL_H
#define FS_MODEL_H

#include "mesh.hpp"

namespace Firesteel {
    struct Material {
        std::vector<Texture> textures;
        void remove() {
            for (size_t i=0; i < textures.size(); i++)
                textures[i].remove();
        }
    };
    struct Model {
        std::string path;
        std::vector<Material> materials;
        std::vector<Node> nodes;
        std::vector<Mesh> meshes;
        std::string getDirectory() const {
            return path.substr(0, path.find_last_of('\\'));
        }
        std::string getFilename() const {
            return path.substr(path.find_last_of('\\')+1, path.size()-path.find_last_of('\\')-1);
        }
        Model(const std::string& tPath="") {
            materials.clear();
            meshes.clear();
            path=tPath;
        }
    };
}

#endif // !FS_MODEL_H