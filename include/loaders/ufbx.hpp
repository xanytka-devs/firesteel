#ifndef FS_LOADER_OFBX
#ifndef FS_LOADERS_FBX
#define FS_LOADERS_FBX

#include "../model.hpp"
#include "../utils/stbi_global.hpp"

#include "../../external/model_loaders/ufbx/ufbx.h"

namespace Firesteel {
    namespace FBX {
        /// [!WARING]
        /// This function is internal and only used for the GLTF loader. Use it at your own risk.
        Mesh processPrimitive(const Model* tBaseModel, const ufbx_mesh* tMesh, const ufbx_mesh_part* tPart
#ifdef FS_PRINT_DEBUG_MSGS
            , size_t& tVert, size_t& tInd, size_t& tNorm, size_t& tTex
#endif
            ) {
            //Initialize variables.
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            //Get textures.
            std::vector<Texture> textures;

            return Mesh(vertices,indices,textures);
        }

        Model load(const std::string& tPath) {
#ifdef FS_INCLUDE_NVTX
            nvtx3::scoped_range r{"fbx model load"};
#endif // FS_INCLUDE_NVTX
            Model model{ tPath };

            ufbx_error error{};
            ufbx_load_opts options{};
            options.ignore_missing_external_files=true;
            ufbx_scene* scene=ufbx_load_file(tPath.c_str(),&options,&error);
            //Check errors.
            if(error.type!=UFBX_ERROR_NONE) {
                LOG_ERRR("Got error while loading model (at "+tPath+"): "+error.info);
                return model;
            }
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Meshes: "+std::to_string(scene->meshes.count));
            LOG_DBG("Materials: "+std::to_string(scene->materials.count));
            size_t vert=0;
            size_t ind=0;
            size_t norm=0;
            size_t tex=0;
#endif // FS_PRINT_DEBUG_MSGS
            //Process all meshes.
            for(size_t m=0;m<scene->meshes.count;m++) {
#ifdef FS_PRINT_DEBUG_MSGS
                LOGF_DBG("Processing mesh %i/%i\n",m+1,scene->meshes.count);
                //model.meshes.push_back(processMesh(&model,scene->getMesh(static_cast<int>(m)),&materialIds,vert,ind,norm,tex));
#else
                //model.meshes.push_back(processMesh(&model,scene->getMesh(static_cast<int>(m)),&materialIds));
#endif // FS_PRINT_DEBUG_MSGS
            }
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Vertices: "+std::to_string((int)(vert) / 3));
            LOG_DBG("Normals: "+std::to_string((int)(norm) / 3));
            LOG_DBG("UVs: "+std::to_string((int)(tex) / 2));
#endif // FS_PRINT_DEBUG_MSGS

            return model;
        }
    }
}

#endif // !FS_LOADERS_FBX
#endif // !FS_LOADER_OFBX