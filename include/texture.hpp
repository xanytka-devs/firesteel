#ifndef FS_TEXTURE_H
#define FS_TEXTURE_H

#include "common.hpp"

namespace Firesteel {
    struct Texture {
        unsigned int ID = 0;
        std::string type;
        std::string path;
        bool isMonochrome = false;
        void bind(size_t tId = 0) const {
            glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(tId));
            glBindTexture(GL_TEXTURE_2D, ID);
        }
        static void unbind() {
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE0);
        }
        void remove() const {
            glDeleteTextures(1, &ID);
        }
    };
}

#endif // !FS_TEXTURE_H
