#ifdef FS_LOADER_OBJ
#ifndef FS_LOADERS_OBJ_H
#define FS_LOADERS_OBJ_H

#include <firesteel/rendering/mesh.hpp>

namespace Firesteel {
    namespace OBJ {
        Model load(std::string tPath);
    }
}

#endif // !FS_LOADERS_OBJ_H
#endif // FS_LOADER_OBJ