#ifndef FS_ENVIROMENT
#define FS_ENVIROMENT

#include <memory>
#include <firesteel/renderer.hpp>

namespace Firesteel {
    class Enviroment {
    public:
        static Enviroment* sInstance() {
            static Enviroment instance;
            return &instance;
        }
        static Renderer* getRenderer() { return sInstance()->renderer.get(); }

        std::unique_ptr<Renderer> renderer;
    };
    
}

#endif // !FS_ENVIROMENT