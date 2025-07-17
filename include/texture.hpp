#ifndef FS_TEXTURE_H
#define FS_TEXTURE_H
#include "common.hpp"

namespace Firesteel {
    enum TextureType {
        TT_DIFFUSE,
        TT_NORMAL,
        TT_SPECULAR,
        TT_EMISSIVE,
        TT_AMBIENT,
        TT_DISPLACEMENT,
        TT_OPACITY
    };
    struct Texture {
        unsigned int ID=0;
        TextureType type;
        std::string path;
        bool isMonochrome=false;
        
        void enable(const size_t& tId=0) const {
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

        const char* typeToString() {
            switch(type) {
            case TT_AMBIENT:
                return "ambient";
            case TT_DIFFUSE:
                return "diffuse";
            case TT_DISPLACEMENT:
                return "displacement";
            case TT_EMISSIVE:
                return "emissive";
            case TT_NORMAL:
                return "normal";
            case TT_OPACITY:
                return "opacity";
            case TT_SPECULAR:
                return "specular";
            default:
                return "unknown";
            }
        }
    };
}

#endif // !FS_TEXTURE_H
