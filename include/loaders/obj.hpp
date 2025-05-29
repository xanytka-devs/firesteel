#ifndef FS_LOADERS_OBJ
#define FS_LOADERS_OBJ

#define TINYOBJLOADER_IMPLEMENTATION
#include "../../external/tiny_obj_loader.h"

#include "../model.hpp"

namespace Firesteel {
    namespace OBJ {
        Model load(std::string tPath) {
            Model model{};

            tinyobj::ObjReaderConfig reader_config;
            reader_config.mtl_search_path = ""; // Path to material files

            tinyobj::ObjReader reader;

            if (!reader.ParseFromFile(tPath, reader_config)) {
              if (!reader.Error().empty())
                  LOG_ERRR("TinyObjReader: " + reader.Error());
              return model;
            }

            if (!reader.Warning().empty())
              LOG_WARN("TinyObjReader: " + reader.Warning());

            auto& attrib = reader.GetAttrib();
            auto& shapes = reader.GetShapes();
            auto& materials = reader.GetMaterials();

            printf("# of materials = %d\n", (int)materials.size());
            printf("# of shapes    = %d\n", (int)shapes.size());

            size_t vert=0;
            size_t ind=0;
            size_t norm=0;
            size_t tex=0;

            //Loop over shapes.
            for(size_t s = 0; s < shapes.size(); s++) {
                std::vector<Vertex> vertexes;
                std::vector<unsigned int> indices;
                std::vector<Texture> textures;

                size_t index_offset = 0;
                vert += shapes[s].mesh.num_face_vertices.size();
                ind += shapes[s].mesh.indices.size();
                //Loop over faces (polygons).
                for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                    size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
                    //Loop over vertices in the face.
                    for(size_t v = 0; v < fv; v++) {
                        Vertex vertex{};
                        glm::vec3 vector{};
                        //Access to vertex.
                        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                        vector.x = attrib.vertices[3*size_t(idx.vertex_index)+0];
                        vector.y = attrib.vertices[3*size_t(idx.vertex_index)+1];
                        vector.z = attrib.vertices[3*size_t(idx.vertex_index)+2];
                        vertex.position = vector;
                        //Check if `normal_index` is zero or positive. negative = no normal data.
                        if(idx.normal_index >= 0) {
                            vector.x = attrib.normals[3*size_t(idx.normal_index)+0];
                            vector.y = attrib.normals[3*size_t(idx.normal_index)+1];
                            vector.z = attrib.normals[3*size_t(idx.normal_index)+2];
                            vertex.normal = vector;
                            norm+=1;
                        }
                        //Check if `texcoord_index` is zero or positive. negative = no texcoord data.
                        if(idx.texcoord_index >= 0) {
                            vector.x = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                            vector.y = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                            vertex.uv = vector;
                            tex+=1;
                        }
                        //Optional: vertex colors
                        //tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                        //tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                        //tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
                        vertexes.push_back(vertex);
                    }
                    index_offset += fv;
                    //Per-face material.
                    materials[shapes[s].mesh.material_ids[f]].diffuse_texname;
                }
                model.meshes.push_back(Mesh(vertexes, indices, textures));
            }

            printf("# of vertices  = %d\n", (int)(vert)/3);
            printf("# of normals   = %d\n", (int)(norm)/3);
            printf("# of texcoords = %d\n", (int)(tex)/2);

            return model;
        }
    }
}

#endif // !FS_LOADERS_OBJ