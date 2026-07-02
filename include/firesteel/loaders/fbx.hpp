#ifndef FS_LOADERS_FBX
#define FS_LOADERS_FBX

#include <firesteel/rendering/mesh.hpp>

namespace Firesteel {
    namespace FBX {
        Model load(const std::string& tPath);
    }
}

#endif // !FS_LOADERS_FBX