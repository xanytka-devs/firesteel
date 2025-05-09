#ifndef FS_LOADER_OBJ
#define FS_LOADER_OBJ

#include "../utils/log.hpp"
#include "../model.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../../external/tiny_obj_loader.h"
#include "../utils/stbi_global.hpp"

namespace Firesteel {
    namespace Loaders {
        namespace OBJ {
            Model load(const std::string& tPath) {
                //Initialize model.
                Model model = Model(tPath);
                tinyobj::attrib_t inattrib;
                std::vector<tinyobj::shape_t> inshapes;
                std::vector<tinyobj::material_t> materials;
                //Load and check for warnings and errors.
                std::string warn, err;
                bool ret = tinyobj::LoadObj(&inattrib, &inshapes, &materials, &warn, &err, tPath.c_str(), model.getDirectory().c_str());
                if(!warn.empty()) LOG_WARN("Got warning \"" + warn + "\" while loading model at \"" + tPath + "\"");
                if(!err.empty()) {
                    LOG_ERRR("Got error \"" + err + "\" while loading model at \"" + tPath + "\"");
                    return model;
                }
                //Print model info.
                printf("Model loading info:\n");
                printf("# of vertices  = %d\n", (int)(inattrib.vertices.size()) / 3);
                printf("# of normals   = %d\n", (int)(inattrib.normals.size()) / 3);
                printf("# of texcoords = %d\n", (int)(inattrib.texcoords.size()) / 2);
                printf("# of materials = %d\n", (int)materials.size());
                printf("# of shapes    = %d\n", (int)inshapes.size());
                printf("\n");
                //Print material info.
                if(materials.size()==0) materials.push_back(tinyobj::material_t());
                for (size_t i = 0; i < materials.size(); i++)
                  printf("material[%d].diffuse_texname = %s\n", int(i),
                         materials[i].diffuse_texname.c_str());
                //Parse meshes.
                return model;
            }
        }
    }
}

#endif // !FS_LOADER_OBJ