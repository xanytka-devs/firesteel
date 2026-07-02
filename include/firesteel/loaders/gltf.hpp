#ifdef FS_LOADER_GLTF
#ifndef FS_LOADERS_GLTF_H
#define FS_LOADERS_GLTF_H

#include <firesteel/rendering/mesh.hpp>

namespace Firesteel {
    namespace GLTF {
        Model load(std::string tPath, bool tBinary);
    }
}

#endif // !FS_LOADERS_GLTF_H
#endif // FS_LOADER_GLTF