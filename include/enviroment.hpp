#ifndef FS_ENVIROMENT
#define FS_ENVIROMENT

#include <memory>
#include "renderer.hpp"

namespace Firesteel {
    class Enviroment {
    public:
        static Enviroment* sInstance;
        std::unique_ptr<Renderer> renderer;
        const char* getVersion() { return mVersion; }
    private:
        const char* mVersion=FS_VERSION;
    };
    
}

Firesteel::Enviroment* Firesteel::Enviroment::sInstance = nullptr;

#endif // !FS_ENVIROMENT